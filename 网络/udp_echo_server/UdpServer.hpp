#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>   
#include <cerrno>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "nocopy.hpp"
#include "Comm.hpp"
#include "log.hpp"

const static std::string defaultip = "0.0.0.0";
const static uint16_t defaultport = 8888;
const static int defaultfd = -1; 
const static int defaultsize = 1024;

class UdpServer : public nocopy
{
public:
    UdpServer(const std::string& ip = defaultip, uint16_t port = defaultport)
    :_ip(ip), _port(port), _sockfd(defaultfd)
    {}
    void Init()
    {
        //1. 创建socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd  < 0)
        {
            lg.LogMessage(Fatal, "socket error , %d, : %s\n", errno, strerror(errno));
            exit(Socket_Err);
        }

        lg.LogMessage(Info, "socket success, sockfd: %d\n", _sockfd);

        //2. 绑定，指定网络信息
        struct sockaddr_in local;
        bzero(&local,sizeof(local));//memset
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = inet_addr(_ip.c_str()); 

        int n = ::bind(_sockfd, (struct sockaddr*)&local, sizeof(local));

        if (n != 0)
        {
            lg.LogMessage(Fatal, "bind error , &d : %s\n", errno, strerror(errno));
            exit(Bind_Err);
        }

    }
    void Start()
    {
        char Buffer[defaultsize];
        //服务器永远不退出
        for(;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);//不能乱写
            ssize_t n = recvfrom(_sockfd, Buffer, sizeof(Buffer)-1, 0, (struct sockaddr*)&peer, &len);
            if (n > 0)
            {
                Buffer[n] = 0;
                std::cout << "client say# " << Buffer << std::endl;
                sendto(_sockfd, Buffer, strlen(Buffer), 0, (struct sockaddr*)&peer, len);
            }
        }
    }
   
    ~UdpServer(){}
private:
    std::string _ip;
    uint16_t _port;
    int _sockfd;
};