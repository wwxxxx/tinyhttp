#ifndef EPOLL_H
#define EPOLL_H

#include "Handler.h"
#include <iostream>
#include <sys/epoll.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

class EventLoop;

class Epoll
{
  public:
    explicit Epoll(EventLoop *loop)
        : ownerLoop(loop),
          epollfd(epoll_create1(EPOLL_CLOEXEC)),
          retEvents(8) // 初始时为epoll_wait预留8个返回的epoll_event
    {
        //FIXME: 调用日志库写入日志并终止程序
        if (epollfd < 0)
            cout << "Epoll::epoll_create1() error: " << ::strerror(errno) << endl;
        assert(epollfd > 0);
    }

    ~Epoll() { close(epollfd); }

    //调用epoll_wait，并将其交给Event类的handleEvent函数处理
    void epoll(vector<Handler *> &events);
    void removeFd(const int fd);
    /*
    void assertInLoopThread() const 
    {
        ownerLoop->assertInLoopThread();
    }
    */
    void addToEpoll(const int fd);

  private:
    EventLoop *ownerLoop;
    int epollfd;
    vector<struct epoll_event> retEvents;
    //vector<Event> events;
};

#endif