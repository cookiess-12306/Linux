#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <sys/epoll.h>
#include "Epoller.hpp"
#include "Socket.hpp"
#include "Log.hpp"

using namespace Net_Work;

const static int gbacklog = 8;
const static int num = 64;
class EpollServer
{
public:
    EpollServer(uint16_t port) : _port(port),
                                 _listensock(new TcpSocket()),
                                 _epoller(new Epoller()),
                                 _isrunning(false)
    {
    }

    void Init()
    {
        // 1. 创建套接字
        _listensock->BuildListenSocketMethod(_port, gbacklog);
        lg.LogMessage(Info, "listensock create success, listensock : %d\n", _listensock->GetSockFd());
        // 2.创建epoll模型
        _epoller->EpollInit();
        lg.LogMessage(Info, "epoller create success\n");
        // 3.添加监听套接字到epoll中
        _epoller->EpollAdd(_listensock->GetSockFd(), EPOLLIN);
    }

    bool Accepter(std::string *clientip, uint16_t *clientport)
    {
        int sock = _listensock->AcceptConnection(clientip, clientport);
        if (sock == -1)
        {
            lg.LogMessage(Error, " accept error..\n");
            return false;
        }
        _epoller->EpollAdd(sock, EPOLLIN);
        return true;

    }

    int Recver(int sockfd, std::string* message)
    {   
        char buffer[1024];
        int n = ::recv(sockfd, buffer, sizeof(buffer), 0);
        if (n > 0)
        {
            buffer[n] = 0;
            *message = buffer;
        }
        return n;
    }

    void HandlerEvent(int n)
    {
        lg.LogMessage(Debug, "%d 个 events 已经就绪\n", n);
        for (int i = 0; i < n; i++)
        {
            lg.LogMessage(Debug, "ready fd : %d , Event is : %u\n", _resv[i].data.fd, _resv[i].events);
            int sock = _resv[i].data.fd;
            uint32_t events = _resv[i].events;
            if (events & EPOLLIN)
            {
                if (sock == _listensock->GetSockFd())
                {
                    std::string clientip;
                    uint16_t clientport;
                    if (!Accepter(&clientip, &clientport))
                        continue;
                    lg.LogMessage(Info, "link success, client info is %s:%d, fd : %d\n", clientip.c_str(), clientport, sock);
                }
                else
                {
                    std::string message;
                    int n = Recver(sock, &message);
                    if (n > 0)
                    {
                        std:: cout << "client# " << message << std::endl;
                    }
                    else 
                    {
                        if (n == 0)
                        {
                            lg.LogMessage(Warning, "client %d close\n", sock);
                        }
                        else
                        {
                            lg.LogMessage(Error, "client %d recv error\n", sock);
                        }
                        _epoller->EpollDel(sock);
                        close(sock);
                    }
                }
            }
        }
    }

    void Loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            int timeout = -1;
            int n = _epoller->Wait(_resv, num, timeout);
            switch (n)
            {
            case 0:
                lg.LogMessage(Info, "epoll timeout...\n");
                break;
            case -1:
                lg.LogMessage(Error, "epoll error!!!\n");
                break;
            default:
                // 正常的就绪的fd
                lg.LogMessage(Info, "epoll success, begin event handler\n");
                HandlerEvent(n);
                break;
            }
        }
        _isrunning = false;
    }

    ~EpollServer()
    {
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
    std::unique_ptr<Epoller> _epoller;
    struct epoll_event _resv[num];
    bool _isrunning;
};