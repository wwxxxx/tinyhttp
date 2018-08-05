#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <functional>
#include "Thread.h"
#include "EventLoop.h"

class EventLoopThread
{
  public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *getLoop();

  private:
    static void *threadFunc(void *arg);
    EventLoop *_loop;
    Thread _thread;
};

#endif