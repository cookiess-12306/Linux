#include <pthread.h>
#include "RingQueue.hpp"

using namespace std;
void* Producter(void* args)
{

}

void* Consumer(void* args)
{
    
}

int main()
{

    pthread_t c, p;
    RingQueue<int>* rp = new RingQueue<int>();

    pthread_create(&p, nullptr, Producter, rp);
    pthread_create(&c, nullptr, Consumer, rp);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;

}