#include "Socket.h"
using std::cout;
using std::endl;

//打开地址复用功能
void Socket::setReuseAddr(const int fd, bool on)
{
    int optval = on ? 1 : 0;
    //第二个参数level是被设置的选项的级别,如果想要在套接字级别上设置选项,就必须把level设置为 SOL_SOCKET,第三个参数SO_REUSEADDR,打开或关闭地址复用功能.
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

//关闭延迟
void Socket::setTcpNoDelay(const int fd, bool on)
{
    int optval = on ? 1 : 0;
    //TCP_NODELAY,在套接字上禁用Nagle算法 (只适用于流式套接字)
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
}

//创建socket
int Socket::createSocket()
{
    //socket描述符是一个指向内部数据结构的指针，它指向描述符表入口
    //AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合
    //SOCK_STREAM -- TCP类型，保证数据顺序及可靠性；
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        // 写入日志
        cout << "Socket::createSocket error: " << strerror(errno) << endl;
        exit(1);
    }
    // setNonBlockAndCloseOnExec(sockfd);
    return sockfd;
}

//绑定socket到本地地址和端口，通常由服务端调用
void Socket::Bind(const int sockfd, const struct sockaddr_in &addr)
{
    int ret = bind(sockfd, (struct sockaddr *)(&addr), sizeof(addr));
    if (ret < 0)
    {
        cout << "Socket::Bind error: " << strerror(errno) << endl;
        exit(1);
    }
}

//listen()函数仅被TCP类型的服务器程序调用,实现监听功能,调用后tcp状态机由close转换到listen
void Socket::Listen(const int sockfd)
{
    if (listen(sockfd, 5) < 0)
    {
        cout << "Socket::Listen error: " << strerror(errno) << endl;
        exit(1);
    }
}

//accept()函数仅被TCP类型的服务器程序调用，从已完成连接队列返回下一个建立成功的连接，
//如果已完成连接队列为空，线程进入阻塞态睡眠状态。
int Socket::Accept(const int sockfd, struct sockaddr_in *addr)
{
    socklen_t addrLen = sizeof(*addr);
    // int connfd = accept(sockfd, (struct sockaddr *)&addr, &addrLen);
    // setNonBlockAndCloseOnExec(connfd);
    int connfd = accept4(sockfd, (struct sockaddr *)&addr,
                         //  &addrLen, SOCK_CLOEXEC);
                         &addrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (connfd < 0)
    {
        switch (errno)
        {
        // 非致命错误，忽略
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EMFILE:
            break;
        // 致命错误，退出程序
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOMEM:
            cout << "Socket::Accept error: " << strerror(errno) << endl;
            exit(1);
            break;
        // 未知错误，退出程序
        default:
            cout << "Socket::Accept error: " << strerror(errno) << endl;
            exit(1);
            break;
        }
    }
}

//关闭连接
void Socket::Close(const int sockfd)
{
    if (close(sockfd) < 0)
    {
        cout << "Socket::Close error : " << strerror(errno) << endl;
        exit(1);
    }
}

void Socket::setNonBlockAndCloseOnExec(const int sockfd)
{
    // 这段代码有问题，一直无法成功执行(Bad file descriptor)，并且
    // fcntl函数返回-1，不知道为什么。所以用到这个函数的地方就用别的方法
    // 替代了，如socket和accpet4。
    int flags;
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0)
    {
        std::cout << "Socket::setNonBlockAndCloseOnExec error: " << strerror(errno) << std::endl;
        exit(1);
    }
    flags |= O_NONBLOCK;
    flags |= FD_CLOEXEC;
    if (fcntl(sockfd, F_SETFL, flags) < 0)
    {
        std::cout << "Socket::setNonBlockAndCloseOnExec error: " << strerror(errno) << std::endl;
        exit(1);
    }
}