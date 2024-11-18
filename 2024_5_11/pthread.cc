#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <functional>
#include <string>
#include <vector>

using namespace std;

//int g_val = 100;

__thread int g_val = 100;

void* ThreadRoutine(void* arg)
{
    string name = static_cast<const char*>(arg);
    while (true)
    {
        sleep(1);
        cout << name << "g_val: " << g_val << " , " << "&g_val" << &g_val << "\n" << endl;
        g_val++;
    }

    return nullptr;
}


int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, ThreadRoutine, (void*)"thread-1");


    while (true)
    {
        sleep(1);
        cout << "main thread g_val: " << g_val << " , " << "&g_val" << &g_val << "\n" << endl;
    }

    pthread_join(tid,nullptr);
    return 0;
}

// void myrun()
// {
//     while (true)
//     {
//         cout << "I am thread" << endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     thread t(myrun);

//     t.join();

//     return 0;
// }