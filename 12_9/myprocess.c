#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    printf("pid = %d\n", getpid());
    pid_t id = fork();
    if (id == 0)
    {
        sleep(1);
        execl("./mytest", "-a","-b","-c", NULL);
     //   char *argv[] = {(char*)"ls",(char*)"-a",(char*)"-l" }; 
     //   //         execl("/usr/bin/ls","ls","-a", "-l", NULL);
     //    sleep(3);        
     //    execv("/usr/bin/ls", argv);
     //    exit(1);
    }//
    pid_t rid = waitpid(id, NULL, 0);
    if (rid > 0)
    {
        printf("wait sucess\n");
    }
    

   
    exit(1);
}
