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
#include <functional>
#include "nocopy.hpp"
#include "Comm.hpp"
#include "log.hpp"
#include "Inetaddr.hpp"

const static std::string defaultip = "0.0.0.0";
const static uint16_t defaultport = 8888;
const static int defaultfd = -1; 
const static int defaultsize = 1024;

using func_t = std::function<std::string(std::string)>;
 
class UdpServer : public nocopy
{
public:
    UdpServer(func_t OnMessage,  uint16_t port = defaultport)
    : _port(port), _sockfd(defaultfd),_OnMessage(OnMessage)
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
        local.sin_addr.s_addr = INADDR_ANY;
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); 

        int n = ::bind(_sockfd, (struct sockaddr*)&local, sizeof(local));

        if (n != 0)
        {
            lg.LogMessage(Fatal, "bind error , &d : %s\n", errno, strerror(errno));
            exit(Bind_Err);
        }

    }
    void Start()
    {
        char buffer[defaultsize];
        //服务器永远不退出
        for(;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);//不能乱写
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
            if (n > 0)
            {
                Inetaddr addr(peer);
                buffer[n] = 0;
                // std::cout << "[" << addr.PrintDebug() << "]# " << Buffer << std::endl;

                std::string response = _OnMessage(buffer);
 
                sendto(_sockfd, response.c_str(), response.size(), 0, (struct sockaddr*)&peer, len);
            }
        }
    }
   
    ~UdpServer(){}
private:
    // std::string _ip;  
    uint16_t _port;
    int _sockfd;

    func_t _OnMessage;
};