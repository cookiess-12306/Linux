#include "Select.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port\n" << std::endl;
    }

    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<SelectServer> svr = std::make_unique<SelectServer>(localport);
    svr->Init();
    svr->Loop();
    return 0;
}