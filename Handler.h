#ifndef HANDLER_H
#define HANDLER_H

#include "Parser.h"
#include "Buffer.h"
#include <string>
#include <algorithm>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using std::cout;
using std::endl;
using std::string;
using std::to_string;

class Handler
{
public:
  Handler(const int connfd);
  ~Handler();
  void handle();
  const int connFd() const;

private:
  bool receiveRequest(); // 接受客户的请求并解析
  void sendResponse();   // 发送响应
  void sendErrorMsg(const string &errorNum,
                    const string &shortMsg,
                    const string &longMsg);
  void parseURI();
  void getFileType();
  int _connfd;
  bool _isClosed;
  string _fileType;
  string _fileName;
  Buffer _inputBuffer;
  Buffer _outputBuffer;
  HTTPRequest _request;
};

#endif