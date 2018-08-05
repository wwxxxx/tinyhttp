#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
using std::copy;
using std::string;
using std::vector;

class Buffer
{
  public:
    Buffer() : _readIndex(0), _writeIndex(0) {}
    size_t readableBytes();
    size_t writableBytes();
    string readAllAsString();
    void append(const char *data, const size_t len);
    char *peek();
    size_t readFd(const int fd);
    void sendFd(const int fd);

  private:
    const char *begin() const
    {
        return &*_buffer.begin();
    }
    char *beginWrite()
    {
        return &*_buffer.begin() + _writeIndex;
    }
    void resetBuffer()
    {
        _readIndex = _writeIndex = 0;
        _buffer.clear();
        //请求容器减小容量使其足够用来存放所有有效元素
        _buffer.shrink_to_fit();
    }
    void makeSpace(const size_t len)
    {
        if (writableBytes() < len)
            _buffer.resize(_writeIndex + len);
    }

    vector<char> _buffer;
    size_t _readIndex;
    size_t _writeIndex;
};

#endif