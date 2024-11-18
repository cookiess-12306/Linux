#pragma once
#include <sys/select.h>
#include <iostream>
#include <memory>
#include "Socket.hpp"
#include "Log.hpp"
#include <unordered_map>
#include <poll.h>

using namespace Net_Work;

const static int gdefaultport = 8888;
const static int backlog = 8;
const static int num = sizeof(fd_set) * 8;
class SelectServer
{
private:
    void HandlerEvent(fd_set &rfds)
    {
        for (int i = 0; i < num; i++)
        {
            if (array_rfds[i] == nullptr)
                continue;
            int fd = array_rfds[i]->GetSockFd();
            if (FD_ISSET(fd, &rfds))
            {
                //新连接到来
                if (fd == _listensock->GetSockFd())
                {
                    std::string clientip;
                    uint16_t clientport;
                    Socket *sock = _listensock->AcceptConnection(&clientip, &clientport);
                    if (sock == nullptr)
                    {
                        lg.LogMessage(Error, " accept error..\n");
                        return;
                    }
                    lg.LogMessage(Info, "link success, client info is %s:%d, fd : %d\n", clientip.c_str(), clientport, sock->GetSockFd());
                    // 此时已经成功获取到了sock套接字，但不能直接read或者write，因为不清楚新的sock上的读事件是否就绪了
                    // 所以，需要将新来的sock交给select保管
                    int i = 0;
                    for (; i < num; i++)
                    {
                        if (array_rfds[i] == nullptr)
                        {
                            array_rfds[i] = sock;
                            lg.LogMessage(Info, "get a new link, fd : %d\n", sock->GetSockFd());
                            break;
                        }
                    }
                    if (i == num)
                    {
                        sock->CloseSocket();
                        delete sock;
                        lg.LogMessage(Warning, "server is full...\n");
                    }
                }
                //普通读事件
                else 
                {
                    std::string buffer;
                    bool res = array_rfds[i]->Recv(&buffer,1024);
                    //lg.LogMessage(Info, "normoal read, buffer : %s\n", buffer.c_str());
                    if (res)
                    {
                        lg.LogMessage(Info, "client say# %s\n", buffer.c_str());
                        // std::string resp;
                        // if (dict.find(buffer.c_str()) == dict.end())
                        //     resp = "not found";
                        // else 
                        //     resp = dict.find(buffer)->second;
                        // array_rfds[i]->Send(resp);
                        buffer.clear();

                    }
                    else 
                    {
                        lg.LogMessage(Warning, "client quit, maybe close or error, close fd : %d\n", array_rfds[i]->GetSockFd());
                        array_rfds[i]->CloseSocket();
                        delete array_rfds[i];
                        array_rfds[i] = nullptr;
                    }
                }
            }
        }
    }

public:
    SelectServer(int port = gdefaultport) : _port(port), _listensock(new TcpSocket()), _isrunning(false)
                                            ,dict({{"hello", "你好"}})
    {
        
    }

    void Init()
    {
        _listensock->BuildListenSocketMethod(_port, 8);
        for (int i = 0; i < num; i++)
        {
            array_rfds[i] = nullptr;
        }
        array_rfds[0] = _listensock.get();
    }

    void Loop()
    {
        _isrunning = true;
        while (_isrunning == true)
        {
            // 先创建一个fd_set
            fd_set rfds;
            FD_ZERO(&rfds);
            // 遍历数组，1.找到最大的fd，2.将合法的fd放到rfds中
            int max_fd = _listensock->GetSockFd();
            for (int i = 0; i < num; i++)
            {
                if (array_rfds[i] == nullptr)
                    continue;
                else
                {
                    int fd = array_rfds[i]->GetSockFd();
                    FD_SET(fd, &rfds);
                    if (fd > max_fd)
                    {
                        max_fd = fd;
                    }
                }
            }
            PrintDebug();
            struct timeval timeout = {5, 0};
            int n = select(max_fd + 1, &rfds, nullptr, nullptr, /*&timeout*/ nullptr);
            switch (n)
            {
            case 0:
                lg.LogMessage(Info, "select timeout..., n : %d, Fd : %d\n", n, _listensock->GetSockFd() + 1);
                break;
            case -1:
                lg.LogMessage(Error, "select error!!!\n");
                break;
            default:
                // 正常的就绪的fd
                lg.LogMessage(Info, "select success, begin event handler\n");
                HandlerEvent(rfds);
                break;
            }
        }
        _isrunning = false;
    }

    void PrintDebug()
    {
        std::cout << "current select rfds list is : ";
        for (int i = 0; i < num; i++)
        {
            if (array_rfds[i] == nullptr)
                continue;
            else
                std::cout << array_rfds[i]->GetSockFd() << " ";
        }
        std::cout << std::endl;
    }

    ~SelectServer()
    {
    }

private:
    std::unique_ptr<Socket> _listensock;
    int _port;
    int _isrunning;

    std::unordered_map<std::string, std::string> dict;
    Socket *array_rfds[num];
};
