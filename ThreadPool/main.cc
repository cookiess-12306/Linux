#include "ThreadPool.hpp"

int Add(int x, int y)
{
    return x+y;
}

int main()
{
    ThreadPool pool(10);
    for (int i = 0 ;i < 10; i++)
    {
        std::future<int> fu = pool.push(Add, i,10);
        pool.PrintDebug();
        std::cout << fu.get() << std::endl;
    }

    pool.stop();
    return 0;
}