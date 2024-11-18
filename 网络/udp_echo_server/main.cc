#include "UdpServer.hpp"
#include <memory>
using namespace std;

void Usage(string proc)
{
    cout << "Usage : \n\t" << proc << "local_ip local_port\n" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    string ip = argv[1];
    uint16_t port = stoi(argv[2]);

    unique_ptr<UdpServer> usvr(new UdpServer(ip, port));
    usvr->Init();
    usvr->Start();
    
    return 0;
}