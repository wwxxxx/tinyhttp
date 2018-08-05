#include "EventLoop.h"

class IgnoreSigPipe
{
  public:
    IgnoreSigPipe()
    {
        //默认处理规则SIGPIPE会使client退出
        //设为SIG_IGN可以防止客户端退出
        //这是SIGPIPE交给系统处理
        signal(SIGPIPE, SIG_IGN);
    }
};

//忽略SIGPIPE信号
IgnoreSigPipe initObj;

EventLoop::EventLoop()
    : isLooping(false),
      threadId(CurrentThread::gettid()),
      isQuit(false),
      e(new Epoll(this)) {}

EventLoop::~EventLoop()
{
    assert(!isLooping);
}

void EventLoop::loop()
{
    assert(!isLooping);
    isLooping = true;
    isQuit = false;

    while (!isQuit)
    {
        cout << "----------Looping----------" << endl;
        addToLoop();
        vector<Handler *> activeEvents;
        activeEvents.clear();
        //调用epoll将活动的套接字描述符对应的Handler取出并处理
        e->epoll(activeEvents);
        vector<Handler *>::iterator iter = activeEvents.begin();
        for (; iter != activeEvents.end(); ++iter)
        {
            cout << "----------Handle request----------" << endl;
            //处理客户端请求的入口
            (*iter)->handle();
            e->removeFd((*iter)->connFd());
            delete *iter;
        }
    }
    isLooping = false;
}

void EventLoop::quit()
{
    isQuit = true;
}

//每次将一个套接字描述符添加到描述符数组中
void EventLoop::addToLoop(const int fd)
{
    // e->addToEpoll(fd);
    cout << "----------Add " << fd << " to loop----------" << endl;
    fds.push_back(fd);
}

//将描述符数组中的所有套接字全部添加到epoll中
void EventLoop::addToLoop()
{
    if (fds.empty())
    {
        cout << "----------fds empty----------" << endl;
        return;
    }
    for (int i = 0; i < fds.size(); ++i)
        e->addToEpoll(fds[i]);
    fds.clear();
    cout << "----------Add all fd to loop----------" << endl;
}