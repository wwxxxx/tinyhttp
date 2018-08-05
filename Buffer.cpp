#include "Buffer.h"

size_t Buffer::readableBytes()
{
    return _writeIndex - _readIndex;
}

size_t Buffer::writableBytes()
{
    return _buffer.size() - _writeIndex;
}

string Buffer::readAllAsString()
{
    string str(begin() + _readIndex, readableBytes());
    resetBuffer();
    return str;
}

void Buffer::append(const char *data, const size_t len)
{
    makeSpace(len);
    copy(data, data + len, beginWrite());
    _writeIndex += len;
}

char *Buffer::peek()
{
    return static_cast<char *>(&*_buffer.begin()) + _readIndex;
}

size_t Buffer::readFd(const int fd)
{
    char extrabuf[65536];
    char *ptr = extrabuf;
    int nleft = 65535;
    int nread;
    //把参数fd所指的文件传送count个字节到buf指针所指的内存中
    while ((nread = read(fd, ptr, nleft)) < 0)
    {
        if (errno == EINTR)
            nread = 0;
        else
            return 0;
    }
    append(extrabuf, nread);
    return nread;
}

void Buffer::sendFd(const int fd)
{
    size_t bytesSent = 0;
    size_t bytesLeft = readableBytes();
    char *ptr = peek();
    while (bytesLeft)
    {
        //把参数buf所指的内存写入count个字节到参数fd所指的文件内
        if ((bytesSent = write(fd, ptr, bytesLeft)) < 0)
        {
            if (bytesSent < 0 && errno == EINTR)
                bytesSent = 0;
            else
                return;
        }
        bytesLeft -= bytesSent;
        ptr += bytesSent;
    }
    resetBuffer();
}