#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

void Usage(const string & process)
{
    cout << "Usage: " << process << "server_ip  server_port" << endl;

}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cerr << "socket error" << strerror(errno) << endl;
        return 2;
    }
    string serverip = argv[1];
    uint16_t serverport = stoi(argv[2]);

    cout << "create socket sucess: " << sock << endl;


    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    while (true)
    {
        string inbuffer;
        cout << "please enter# ";
        getline(cin, inbuffer);
        ssize_t n = (sock, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr*)&server, sizeof(server));
        if (n > 0)
        {
            char buffer[1024];
            struct sockaddr_in tmp;
            socklen_t len = sizeof(tmp);
            ssize_t m =  recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&tmp, &len);
            if (m > 0)
            {
                buffer[m] = 0;
                cout << "server echo# " << buffer << endl;
            }
            else
            {
                break;
            }
        }
        else
            break;
    }
    close(sock);
    return 0;
}