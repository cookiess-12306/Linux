#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <functional>
#include <string>
#include <vector>

using namespace std;

// string ToHex(pthread_t tid)
// {
//     char id[64];
//     snprintf(id, sizeof(id), "0x%x", tid);
//     return id;
// }

// class ThreadRetrun
// {
// public:
//     ThreadRetrun(pthread_t tid, const string& Info, const int code)
//     :tid_(tid), Info_(Info), code_(code)
//     {}
// public:
//     pthread_t tid_;
//     string Info_;
//     int code_;
// };

// void* ThreadRotine(void* arg)
// {
//     usleep(1000);
//     string name = static_cast<const char*>(arg);
//     int cnt = 5;
//     // while (true)
//     // {
//     //     sleep(1);
//     //     std::cout << "new thread is running , new thread name is :" << name << "  new thread id : " << ToHex(pthread_self()) << std::endl;
//     // }
//     while (cnt--)
//     {
//         std::cout << "new thread is running , new thread name is :" << name << "  new thread id : " << ToHex(pthread_self()) << std::endl;
//         sleep(1);
//     }
//     ThreadRetrun* ret = new ThreadRetrun(pthread_self(), "thread quit normal", 0);
//     return ret;
//     //return (void*)"thread -1 done";//1.线程终止
//     //pthread_exit((void*)"thread -1 done");//2.线程库函数

//     //exit(13);//进程终止，退出会整个进程结束
// }


//thread id本质是地址
//获取线程返回值
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, ThreadRotine, (void*)"thread -1");

//     std::cout << "main thread is runnning,  main thread id is : " << ToHex(pthread_self()) << std::endl;
//     //sleep(10);
    
//     //线程默认要被等待吗？？？ 要！
//     //1.线程退出，没有等待，会出现类似进程的僵尸问题
//     //2.线程退出时，主线程如何获得新线程的返回值？
//     void* ret = nullptr;
//     int n = pthread_join(tid, &ret);//线程异常退出不考虑异常，因为一个线程异常退出，整个进程直接退出

//     cout << "main thread done " << " n: " << n <<  endl;

//     ThreadRetrun* r = static_cast<ThreadRetrun* >(ret);

//     cout << "main thread get new thread Info: " << r->code_ << "," << ToHex(r->tid_) << "," << r->Info_ << endl;
//     //sleep(5);
//     return 0;
// }


void* ThreadRotine(void* arg)
{
    // pthread_detach(pthread_self());//新线程自我分离
    int cnt = 5;
    while (true)
    {
        cout << "new thread running " << endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, ThreadRotine, (void*)"thread -1");
      
    //线程是可以被设置为分离状态的
    //线程模式joinable的
    int cnt = 10;
    while (true)
    {
        cout << "main pthread running ...." << endl;
        cnt--;
        sleep(1);
        if (cnt == 0)
        {
            //pthread_detach(tid);//主线程分离
            int n = pthread_cancel(tid);//线程取消
            cout << "main thread cancel done " << " n: " << n <<  endl;
    
            void* ret = nullptr;
            n = pthread_join(tid, &ret);//线程异常退出不考虑异常，因为一个线程异常退出，整个进程直接退出
            cout << "main thread join done " << " n: " << n << " thread return : "  << (int64_t)ret <<endl;
        }
        //在主线程中，不管是分离进程后再取消进程， 还是直接取消进程，都不会影响主线程的正常运行
    }
    sleep(5);

    //pthread_detach(tid);//主线程分离

    // int n = pthread_cancel(tid);//线程取消
    // cout << "main thread cancel done " << " n: " << n <<  endl;
    
    // void* ret = nullptr;
    // n = pthread_join(tid, &ret);//线程异常退出不考虑异常，因为一个线程异常退出，整个进程直接退出
    // cout << "main thread join done " << " n: " << n << " thread return : "  << (int64_t)ret <<endl;

    //如果线程是分离了，再进行取消，线程等待可以接收到他的退出信息
    //如果线程没有分离，而是被取消了，线程将直接退出，并且退出信息为-1

    return 0;
}
