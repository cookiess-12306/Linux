#pragma once
#include <iostream>
#include <vector>


const int queue_size = 5;

template <class T>
class RingQueue
{
public:
    RingQueue(int size = queue_size):_ringqueue(size), _size(size)
    {}
    ~RingQueue()
    {}

private:
    std::vector<T> _ringqueue;
    int _size;
};