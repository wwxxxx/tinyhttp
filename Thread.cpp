#include "Thread.h"
#include <pthread.h>
#include <assert.h>
using std::cout;
using std::endl;

void Thread::start()
{
    assert(!started);
    started = true;
    //若成功，返回0；否则，返回错误编码
    if (pthread_create(&pthreadId, NULL, threadFunc, _arg))
        started = false;
    cout << "Thread create" << endl;
}

int Thread::join()
{
    assert(started && !joined);
    joined = true;
    //以阻塞的方式等待thread指定的线程结束,当函数返回时,被等待线程的资源被收回
    return pthread_join(pthreadId, NULL);
}

bool Thread::isstarted()
{
    return started;
}

pthread_t Thread::gettid()
{
    return pthreadId;
}