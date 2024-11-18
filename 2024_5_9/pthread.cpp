#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <functional>
#include <string>
#include <vector>
using namespace std;

using func_t = function<void()>; 

const int Threadnum = 5;

class ThreadDate
{
public:
    ThreadDate(const string& name, const uint64_t& ctime, func_t f)
    :Threadname(name), createtime(ctime), func(f)
    {}
public:
    string Threadname;
    uint64_t createtime;
    func_t func;
};

void Print()
{
    cout << "我是线程执行的大任务部分" << endl;
}

void* ThreadRotine(void* args)
{
    int a = 10;
    ThreadDate* td = static_cast<ThreadDate*>(args);
    while (true)
    {
        std::cout << "new thread" << " thread name: " << td->Threadname << " create time: " << td->createtime <<  std::endl;
        td->func();
        if (td->Threadname == "thread-4")
        {
            cout << td->Threadname << " 触发异常！！！！！！！！" << endl;
            a/=0;
        }
        sleep(1);
        
    }
}

int main()
{
    vector<pthread_t> pthreads;
    for (int i = 0; i < Threadnum; i++)
    {
        char threadname[64];
        snprintf(threadname, sizeof(threadname), "%s-%u", "thread", i);

        pthread_t tid;
        ThreadDate *td = new ThreadDate(threadname, (uint64_t)time(nullptr), Print);
        pthread_create(&tid, nullptr, ThreadRotine, td);
        pthreads.push_back(tid);
        sleep(1);
    }
    
    while (true)
    {
        std::cout << "main thread" << std::endl;
        sleep(3);
    }
    return 0;
}