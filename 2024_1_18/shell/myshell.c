#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define SEP " "
#define SIZE 1024
#define ARGC_MAX 64

char* argv[ARGC_MAX];

const char* HostName()
{
    char* hostname = getenv("HOSTNAME");
    if (hostname) return hostname;
    else return "NONE";
}

const char* UserName()
{
    char* hostname = getenv("USER");
    if (hostname) return hostname;
    else return "NONE";
}

const char* CurrentWorkDir()
{
    char* hostname = getenv("PWD");
    if (hostname) return hostname;
    else return "NONE";
}

int Interactive(char out[], int size)
{
    int i = 0;
    printf("[%s@%s %s]", UserName(), HostName(), CurrentWorkDir());
    fgets(out, size, stdin);
    if (out[0] == '\n') i = 1;
    out[strlen(out)-1] = 0;
    return i;
}

void Split(char in[])
{
    int i = 0;
    argv[i++] = strtok(in, SEP); 
    while(argv[i++] = strtok(NULL, SEP));
    if (strcmp(argv[0], "ls") == 0)
    {
        argv[i-1] =(char*)"--color";
        argv[i] = NULL;
    }
}

void Excute()
{
    pid_t rid = fork();
    if (rid == 0)
    {
        execvp(argv[0], argv);
        exit(1);
    }
    waitpid(rid, NULL, 0);
}

int main()
{
    
    char commandline[SIZE];
    while(1)
    {
        //1.获取字符
        int i = Interactive(commandline, SIZE);
        if (i == 1) continue;
        //2.切割字符
        Split(commandline);
        //3.执行命令
        Excute();
    }
    return 0; 
}
