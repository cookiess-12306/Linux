#include "EpollServer.hpp"


void Usage(std::string process)
{
    std::cerr << "Usage: " << process << " port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
    }
    int port = std::stoi(argv[1]);
    std::unique_ptr<EpollServer> svr = std::make_unique<EpollServer>(port);
    svr->Init();
    svr->Loop();
    return 0;
}