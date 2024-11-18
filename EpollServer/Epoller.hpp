#pragma once
#include <sys/epoll.h>
#include <cstring>
#include <unistd.h>
#include "Socket.hpp"
#include "Log.hpp"

class Epoller
{
    const static int defaultepfd = -1;
    const static int size = 128;

public:
    Epoller():_epfd(defaultepfd)
    {}

    void EpollInit()
    {
        _epfd = ::epoll_create(size);
        if (_epfd == -1)
        {
            lg.LogMessage(Fatal, "epoll create erorr, %s : %d\n", strerror(errno), errno);
            exit(4);
        }
        lg.LogMessage(Info, "epoll create success, epfd : %d\n", _epfd);

    }

    void EpollAdd(int sock, uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = sock;
        ev.events = events;
        int n = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
        if (n != 0)
        {
            lg.LogMessage(Fatal, "epoll create erorr, %s : %d\n", strerror(errno), errno);
            exit(4);
        }
        lg.LogMessage(Info, "epoll ctl success...\n");
    }

    int Wait(struct epoll_event* events, int maxevents, int timeout)
    {
        int n = ::epoll_wait(_epfd, events, maxevents, timeout);
        return n;
    }

    void EpollDel(int sockfd)
    {
        int n = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);
        
    }

    ~Epoller()
    {
        if (_epfd >= 0)
        {
            close(_epfd);
        }
        lg.LogMessage(Info, "epoll close success...\n");

    }
private:
    int _epfd;
};
