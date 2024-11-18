#include "UdpServer.hpp"
#include <memory>
using namespace std;

void Usage(string proc)
{
    cout << "Usage : \n\t" << proc << "local_ip local_port\n" << endl;
}

string OnMessagedefault(string request)
{
    return  request; 
}

bool SafeCheck(string command)
{
    return true;
}

string ExcuteCommand(string command)
{
    if (!SafeCheck(command)) return "no";
    cout << "get a message " << command << endl; 
    FILE* fp = popen(command.c_str(), "r");
    if (fp == nullptr)
    {
        return "excute error";
    }

    string response;
    char buffer[1024];
    while(true)
    {
        char* s = fgets(buffer, sizeof(buffer), fp);
        if (!s) break;
        else response += buffer; 
    }


    pclose(fp);
    return response.empty() ? "sucess": response;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    uint16_t port = stoi(argv[1]);

    unique_ptr<UdpServer> usvr(new UdpServer(OnMessagedefault,port));
    usvr->Init();
    usvr->Start();
    
    return 0;
}