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
const static int gnum = 1024;
class PollServer
{
private:
    void HandlerEvent()
    {
        for (int i = 0; i < _num; i++)
        {
            if (_rfds[i].fd == -1)
                continue;
            int fd = _rfds[i].fd;
            short revents = _rfds[i].revents;
            if (revents & POLLIN)
            {
                //新连接到来
                if (fd == _listensock->GetSockFd())
                {
                    std::string clientip;
                    uint16_t clientport;
                    int sock = _listensock->AcceptConnection(&clientip, &clientport);
                    if (sock == -1)
                    {
                        lg.LogMessage(Error, " accept error..\n");
                        return;
                    }
                    lg.LogMessage(Info, "link success, client info is %s:%d, fd : %d\n", clientip.c_str(), clientport, fd);
                    // 此时已经成功获取到了sock套接字，但不能直接read或者write，因为不清楚新的sock上的读事件是否就绪了
                    // 所以，需要将新来的sock交给select保管
                    int i = 0;
                    for (; i < _num; i++)
                    {
                        if (_rfds[i].fd == -1)
                        {
                            _rfds[i].fd = sock;
                            _rfds[i].events = POLLIN;
                            lg.LogMessage(Info, "get a new link, fd : %d\n", sock);
                            break;
                        }
                    }
                    if (i == _num)
                    {
                        close(sock);
                        lg.LogMessage(Warning, "server is full...\n");
                    }
                }
                //普通读事件
                else 
                {
                    char buffer[1024];
                    ssize_t res = recv(fd, buffer, sizeof(buffer-1), 0);
                    if (res > 0)
                    {
                        buffer[res] = 0;
                        lg.LogMessage(Info, "client say# %s\n", buffer);
                        
                    }
                    else 
                    {
                        lg.LogMessage(Warning, "client quit, maybe close or error, close fd : %d\n", _rfds[i].fd);
                        close(fd);
                        _rfds[i].fd = -1;
                        _rfds[i].events = 0;
                        _rfds[i].revents = 0;
                    }
                }
            }
        }
    }

public:
    PollServer(int port = gdefaultport) : _port(port), _listensock(new TcpSocket()), _isrunning(false), _num(1024)
    {
        dict = {{"hello", "你好"}, {"eat", "吃"}};
    }

    void Init()
    {
        _listensock->BuildListenSocketMethod(_port, 8);
        _rfds = new struct pollfd[_num];
        for (int i = 0; i < _num; i++)
        {
            _rfds[i].fd = -1;
            _rfds[i].events = 0;
            _rfds[i].revents = 0;
        }
        _rfds[0].fd = _listensock->GetSockFd();
        _rfds[0].events |= POLLIN; 
    }

    void Loop()
    {
        _isrunning = true;
        while (_isrunning == true)
        {
            int timeout = -1;
            PrintDebug();
            int n = poll(_rfds, _num, timeout);
            switch (n)
            {
            case 0:
                lg.LogMessage(Info, "poll timeout...\n");
                break;
            case -1:
                lg.LogMessage(Error, "poll error!!!\n");
                break;
            default:
                // 正常的就绪的fd
                lg.LogMessage(Info, "poll success, begin event handler\n");
                HandlerEvent();
                break;
            }
        }
        _isrunning = false;
    }

    void PrintDebug()
    {
        std::cout << "current select rfds list is : ";
        for (int i = 0; i < _num; i++)
        {
            if (_rfds[i].fd == -1)
                continue;
            else
                std::cout << _rfds[i].fd << " ";
        }
        std::cout << std::endl;
    }

    ~PollServer()
    {
    }

private:
    std::unique_ptr<Socket> _listensock;
    int _port;
    int _isrunning;

    std::unordered_map<std::string, std::string> dict;

    struct pollfd* _rfds;
    int _num;
};
