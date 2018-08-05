#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Epoll.h"
#include "CurrentThread.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <assert.h>
#include <vector>
#include <poll.h>
#include <signal.h>
using std::cout;
using std::endl;
using std::vector;

class Handler;

class EventLoop
{
  public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void addToLoop(const int fd);

  private:
    void addToLoop();
    bool isLooping;
    const pid_t threadId;
    vector<int> fds;
    bool isQuit;
    Epoll *e;
};

#endif