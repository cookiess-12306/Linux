#include "Protocol.hpp"
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Calculate.hpp"
#include <memory>
#include <unistd.h>

using namespace Net_Work;
using namespace ProtocolNs;
using namespace CalculateNS;

std::string HandlerRequest(std::string &inbufferstream, bool *error_code)
{
    // 0. 计算机对象
    Calculate calculate;

    // 1.构建响应对象
    std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    auto req = factory->BuildRequest();

    // 2.分析字节流，是否有一个完整的报文
    std::string message;
    std::string total;
    while (Decode(inbufferstream, &message))
    {
        std::cout << message << " -----message " << std::endl; 
        // 3.一定读到了完整报文，可以进行反序列化
        if (!req->Deserialize(message))
        {
            *error_code = false;
            return std::string();
        }
        // 4.业务处理
        auto resp = calculate.Cal(req);
        // 5.序列化response
        std::string send_string;
        resp->Serialize(&send_string);
        // 6.构建完整的字符串级别的响应报文
        send_string = Encode(send_string);
        // 7.发送
        total += send_string;
        std::cout << "total: " << total << std::endl; 
    }
    return total;
}

// ./tcpserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << "port" << std::endl;
        return 0;
    }

    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerRequest));
    svr->Loop();

    return 0;
}