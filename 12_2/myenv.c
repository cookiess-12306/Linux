#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM 5
typedef void(*fun_t)();
fun_t tack[NUM];
//////////////////////////////任务/////////////////////////////
void printlog()
{
    printf("this is a log print task\n");
}

void printNet()
{
    printf("this is a net task\n");
}

void printNPC()
{
  printf("this is a NPC\n");
   
}




int main()
{
    int cnt = 0;
    pid_t id = fork();
    while (cnt)
    {
        printf("pid: %d, ppid: %d, cnt: %d\n", )
    return 0;
}








//
//int main()
//{
//    pid_t id = fork();
//    if (id == 0)
//    {
//        // child
//        int cnt = 5;
//        while(cnt--)
//        {
//            printf("Child is running, pid = %d, ppid = %d\n", getpid(), getppid());
//            sleep(1);
//        }
//        printf("子进程准备退出变成僵尸进程\n");
//        exit(1);
//    }
//    int status = 0;
//    printf("父进程开始回收\n");
//    pid_t rid = waitpid(id,&status,0);
//    if (rid > 0)
//    {
//        printf("wait success pid %d, ppid %d status %d\n",getpid(), getppid(), status);
//    }
//
//    return 0;
//}
//


//int main()
//{
//    printf("hello world\n");
//    sleep(3);
//
//    _exit(1);
//}








//enum{
//    success = 0,
//    open_err,
//    malloc_err
//};
//
////const char* errorToDesc(int code)
////{
////    switch (code)
////        
////}
//
//int main()
//{
//    FILE *fp = fopen(",/log.txt", "r");
//    printf("%d:%s\n", errno, strerror(errno));
//    return 0;
//}
