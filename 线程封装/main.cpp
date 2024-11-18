#include "pthread.hpp"
#include <vector>
#include <cstdio>
using namespace std;

string GetThreadName()
{
    static int number = 1;
    char name[64];
    snprintf(name, sizeof(name), "Thread - %d", number++);
    return name;
}


void  Print(int num)
{
    while (num--)
    {
        cout << "hello world: " << num << endl;
        sleep(1);
    }
}

int ticket = 10000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//加锁：
//1.对尽可能少的代码块加锁
//2.一般加锁，都是给临界区加锁
void GetTicket(string name)
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (ticket > 0)
        {
            usleep(1000);//充当抢票花费的时间
            printf("%s, get a ticket: %d\n", name.c_str(), ticket);
            ticket--;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        
    }
}

int main()
{
    const string name1 = GetThreadName();
    Thread<string> t1(name1, GetTicket, name1);

    const string name2 = GetThreadName();
    Thread<string> t2(name2, GetTicket, name2);

    const string name3 = GetThreadName();
    Thread<string> t3(name1, GetTicket, name3);

    const string name4 = GetThreadName();
    Thread<string> t4(name4, GetTicket, name4);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();

    return 0;
}

// int main()
// {

//     // Thread<int> t(GetThreadName(),Print, 10);

//     // t.Start();
//     // t.Join();


//     // const int num = 5;
//     // vector<Thread> threads;

//     // for (int i = 0; i < num; i++)
//     // {
//     //     threads.push_back(Thread(Print, GetThreadName()));
//     // }

//     // for (auto& t : threads)
//     // {
//     //     cout << "Thread name is : " <<t.ThreadName() << " is running : " << t.IsRunning() << endl;
//     // }

//     // for (auto& t : threads)
//     // {
//     //     t.Start();
//     // }

//     // for (auto& t : threads)
//     // {
//     //     cout << "Thread name is : " <<t.ThreadName() << " is running : " << t.IsRunning() << endl;
//     // }

//     // for (auto& t : threads)
//     // {
//     //     t.Join();
//     // }

//     // Thread t(Print, GetThreadName());
//     // cout << "is thread running" << t.IsRunning() << endl;

//     // t.Start();
//     // cout << "is thread running" << t.IsRunning() << endl;

//     // t.Join();
//     return 0;
// }