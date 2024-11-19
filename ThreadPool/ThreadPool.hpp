#include <iostream>
#include <vector>
#include <memory.h>
#include <functional>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    using Functor = std::function<void(void)>;
    ThreadPool(int threadCount = 1) : _stop(false)
    {
        for (int i = 0; i < threadCount; i++)
        {
            _threads.emplace_back(&ThreadPool::entry, this);
        }
    }
    ~ThreadPool()
    {
        stop();
    }
    void stop()
    {
        if (_stop == true) return;
        _stop = true;
        _cv.notify_all();
        for (auto &thread : _threads)
        {
            thread.join();
        }
    }

    // F表示传入的函数，args表示这个函数要处理的参数
    // push函数内部实现的是将任务封装成一个异步任务 packaged_task
    // 使用lambda表达式生成一个可调用对象，放入任务池中，让工作线程取出使用
    template <typename F, typename... Args>
    auto push(F &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        // 1.将传入的任务封装成一个可调用对象
        using return_type = decltype(func(args...));                               // 推演出的返回值
        auto tmp_func = std::bind(std::forward<F>(func), std::forward<Args>(args)...); // 绑定func函数里的参数，用forward实现完美转发
        // std::packaged_task<T> task(func);
        // 创建一个packaged_task对象，把func封装起来，返回值是return_type，再用share_ptr去管理这个任务对象
        auto task = std::make_shared<std::packaged_task<return_type()>>(tmp_func);
        std::future<return_type> fu = task->get_future();
        // 2.构造一个lambda捕获任务，在函数内执行
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // 3.将任务抛入任务池当中
            _tasks.push_back([task]()
                             { (*task)(); });
            _cv.notify_one();
        }
        return fu;
    }
    void PrintDebug()
    {
        //std::unique_lock<std::mutex> lock(_mutex);
        std::thread::id thread_id = std::this_thread::get_id();
        std::cout << "当前子线程是: " << thread_id << std::endl; 
    }

private:
    // 线程池的入口函数
    void entry()
    {
        while (!_stop)
        {
            // 频繁的加锁取任务会导致锁的冲突
            std::vector<Functor> tmp_tasks;
            // 加锁
            {
                std::unique_lock<std::mutex> lock(_mutex);
                PrintDebug();
                // 等待
                _cv.wait(lock, [this]()
                         { return _stop || !_tasks.empty(); });
                // 取出任务
                tmp_tasks.swap(_tasks);
            }
            for (auto &task : tmp_tasks)
            {
                task();
            }
        }
    }

private:
    std::atomic<bool> _stop;
    std::mutex _mutex;//互斥锁
    std::condition_variable _cv;//条件变量
    std::vector<Functor> _tasks;//任务池
    std::vector<std::thread> _threads;//线程池
};