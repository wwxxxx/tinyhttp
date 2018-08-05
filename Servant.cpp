#include "Socket.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "EventLoopThreadPool.h"
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        return 0;
    }
    int port = atoi(argv[1]);

    int listenFd = Socket::createSocket();
    Socket::setReuseAddr(listenFd, true);
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    //htonl(),将一个32位数从主机字节顺序转换成网络字节顺序
    //就是整数在地址空间存储方式变为高位字节存放在内存的低地址处。
    //INADDR_ANY是0.0.0.0,表示本机的所有IP
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    Socket::Bind(listenFd, servAddr);
    Socket::Listen(listenFd);

    EventLoopThreadPool *threadPool = new EventLoopThreadPool(4);
    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        memset(&clientAddr, 0, sizeof(clientAddr));
        int connFd = Socket::Accept(listenFd, &clientAddr);

        //挑选一个线程，将已连接套接字注册到此线程的EventLoop中
        EventLoopThread *thread = threadPool->getNextThread();
        EventLoop *loop = thread->getLoop();
        loop->addToLoop(connFd);
    }
    Socket::Close(listenFd);
    delete threadPool;
    return 0;
}