#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <memory>

int cnt = 10000;
std::mutex mtx;

void test()
{
    while (true)
    {
        std::unique_lock<std::mutex> _lock(mtx); 
        //mtx.lock();
        if (cnt > 0)
        {
            usleep(100);
            std::cout << cnt << " " << std::this_thread::get_id() << std::endl;
            cnt--;
            _lock.unlock();
            //mtx.unlock();
        }
        else 
        {
            _lock.unlock();
            //mtx.unlock();
            break;
        }
    }
}

int main()
{
    std::vector<std::thread> threads(5);
    //std::mutex mtx;
    for (auto& thread: threads)
    {
        thread = std::thread(test);
    }

    for (auto& thread: threads)
    {
        thread.join();
    }
    return 0;
}