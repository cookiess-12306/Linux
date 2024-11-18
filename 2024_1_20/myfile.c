#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "log.txt"

int main()
{
    



/////////////////////////////////////////////////////////////////////    
    //close(1);//关闭1 号文件，输出重定向
    //int fd = open("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);//设置文件权限
   
    //close(0);
    //open("log.txt", O_RDONLY);//设置文件权限
    //int a = 0;
    //scanf("%d", &a);
    //printf("%d\n",a);
    //return 0;
    //printf("fd: %d\n", fd);
    //printf("%d\n", stdin->_fileno);    
    //printf("%d\n", stdout->_fileno);
    //printf("%d\n", stderr->_fileno);

    //return 0;
///////////////////////////////////////////////////////////////////////////////////////////////    
    //umask(0); //自己设置系统掩码
    //int fd = open("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);//设置文件权限
    //if (fd == -1)
    //{
    //    perror("open");
    //    return 1;
    //}
    //
    //const char* buf = "hello write\n";
    //write(fd, buf, strlen(buf));

    //close(fd);
    //return 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    //FILE *fp = fopen(FILENAME, "w");
    //if (fp == NULL)
    //{
    //    perror("fpoen");
    //    return 1;
    //}

    //const char* a = "hello world\n";
    //while(1)
    //{
    //    int n = fwrite(a, strlen(a), 1, fp);
    //    if (n--) break;
    //}
    //fclose(fp);
    //return 0;
}
