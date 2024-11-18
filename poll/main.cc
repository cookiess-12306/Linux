#include "Select.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port\n" << std::endl;
    }

    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<PollServer> svr = std::make_unique<PollServer>(localport);
    svr->Init();
    svr->Loop();
    return 0;
}