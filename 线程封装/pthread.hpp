#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>

using namespace std;

template<class T>
using func_t = std::function<void(T)>;

template<class T>
class Thread
{
public:
    Thread(const string& threadname, func_t<T> func, T data)
    :_tid(0), _threadname(threadname), _isrunning(false), _func(func),_data(data)
    {}

    static void* ThreadRoutine(void* arg)//类内方法，会自带*this, 不符合要求，需要带上static
    {
        //(void)arg;//防止编译器报警

        Thread *ts = static_cast<Thread*>(arg);

        ts->_func(ts->_data);//因为static导致没有this指针了，无法访问到类内成员函数

        return nullptr;
    }

    bool Start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n == 0) 
        {
            _isrunning = true;
            return true;
        }
        else return false;
    }

    bool Join()
    {
        if (!_isrunning) return true;
        int n = pthread_join(_tid, nullptr);
        if (n == 0)
        {
            _isrunning = false;
            return true;
        }
        return false;
    }

    string ThreadName()
    {
        return _threadname;
    }

    bool IsRunning()
    {
        return _isrunning;
    }

    ~Thread()
    {}

private:
    pthread_t _tid;
    string _threadname;
    bool _isrunning;
    func_t<T> _func;//回调
    T _data;
};