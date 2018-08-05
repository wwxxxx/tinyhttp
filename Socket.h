#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

class InetAddr;
class Socket
{
public:
  // 是否开启SO_REUSEADDR选项
  static void setReuseAddr(const int fd, bool on);
  static void setTcpNoDelay(const int fd, bool on);

  // socket相关操作
  static int createSocket();
  static void Bind(const int sockfd, const struct sockaddr_in &addr);
  static void Listen(const int sockfd);
  static int Accept(const int sockfd, struct sockaddr_in *addr);
  static void Close(const int sockfd);
  static void setNonBlockAndCloseOnExec(const int sockfd);
};

#endif