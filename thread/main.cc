#include <iostream>
#include <thread>
#include <vector>

void work()
{
    std::cout << std::this_thread::get_id() << "   1" << std::endl;
}

int main(int argc, char* argv[])
{
    int thread_count = 1;
    if (argc == 2) thread_count = std::stoi(argv[1]);
    std::vector<std::thread> threads(thread_count);
    for (int i = 0; i < thread_count; i++)
    {
        threads[i] = std::thread(work);
    }
    for (int i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }
       
    return 0;


}