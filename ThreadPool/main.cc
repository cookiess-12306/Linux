#include "ThreadPool.hpp"
#include <unistd.h>

int Add(int x, int y)
{
    return x+y;
}

int main()
{
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout << "主线程是: " << thread_id << std::endl;
    ThreadPool pool(3);
    for (int i = 0 ;i < 1000000; i++)
    {
        usleep(200);
        std::future<int> fu = pool.push(Add, i,10);
        // pool.PrintDebug();
        std::cout << fu.get() << std::endl;
    }

    pool.stop();
    return 0;
}