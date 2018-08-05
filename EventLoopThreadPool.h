#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include "EventLoopThread.h"
#include <vector>
using std::vector;

class EventLoop;

class EventLoopThreadPool
{
  public:
    EventLoopThreadPool(const int threadNum);
    ~EventLoopThreadPool();
    EventLoopThread *getNextThread();

  private:
    int _threadNum;
    int _nextId;
    EventLoop *_baseLoop;
    vector<EventLoopThread *> _threads;
};

#endif