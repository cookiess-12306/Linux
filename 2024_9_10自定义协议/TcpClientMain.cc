#include "Protocol.hpp"
#include "Socket.hpp"
#include <unistd.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace ProtocolNs;

// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage : " << argv[0] << " serverip serverport" << std::endl;
        return 0;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    Net_Work::Socket *s = new Net_Work::TcpSocket();
    if (!s->BuildConnectSocketMethod(serverip, serverport))
    {
        std::cerr << "connect " << serverip << ":" << serverport << " failed" << std::endl;
    }
    std::cout << "connect " << serverip << ":" << serverport << " success" << std::endl;

    std::unique_ptr<Factory> factory = std::make_unique<Factory>();

    srand(time(nullptr) ^ getpid());
    const std::string opers = "+-*/%=&";
    while (true)
    {
        int x = rand() % 100;
        usleep(rand() % 7777);
        int y = rand() % 100;
        char oper = opers[rand() % opers.size()];
        std::shared_ptr<Request> req = factory->BuildRequest(x, y, oper);

        // 对请求进行序列化
        std::string request_str;
        req->Serialize(&request_str);
        std::string testreq = request_str;
        testreq += " ";
        testreq += "= ";
        // 添加报头
        request_str = Encode(request_str);

        // 发送请求
        s->Send(request_str);
        std::string response_str;

        while (true)
        {
            // 读取响应
            if (!s->Recv(&response_str, 1024)) break;

            // 报文解析
            std::string response;
            if (!Decode(response_str, &response))
                continue;
            // 反序列化
            auto resp = factory->BuildResponse();
            resp->Deserialize(response);
            // 输出
            std::cout << testreq << resp->GetResult() << "[" << resp->GetCode() << "]" << std::endl;
            break;

        }
        sleep(1);
    } 
    s->CloseSocket();
    return 0;
}