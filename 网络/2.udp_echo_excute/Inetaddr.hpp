#pragma once

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <strings.h>


using namespace std;
class Inetaddr
{
public:
    Inetaddr(struct sockaddr_in &addr):_addr(addr)
    {
        _port = ntohs(_addr.sin_port);
        _ip = inet_ntoa(_addr.sin_addr);
    
    }

    string Ip(){return _ip;}
    uint16_t Port(){return _port;}

    string PrintDebug()
    {
        string clientInfo = _ip;
        clientInfo += ":";
        clientInfo += to_string(_port);
    }
    
    
    ~Inetaddr()
    {}

private:
    string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};