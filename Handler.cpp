#include "Handler.h"

Handler::Handler(const int connfd) : _connfd(connfd)
{
    _isClosed = false;
}

Handler::~Handler()
{
    if (!_isClosed)
        close(_connfd);
}

void Handler::handle()
{
    if (!receiveRequest())
    {
        close(_connfd);
        _isClosed = true;
        return;
    }
    if (_request.method != "GET")
    {
        sendErrorMsg("501", "Not Implemented",
                     "Server doesn't implement this method");
        return;
    }
    parseURI();
    struct stat fileInfo;
    //stat(),获取一个文件的状态
    if (stat(_fileName.c_str(), &fileInfo) < 0)
    {
        cout << "404 Not found: Server couldn't find this file." << endl;
        sendErrorMsg("404", "Not Found", "Server couldn't find this file");
        return;
    }
    if (!(S_ISREG(fileInfo.st_mode)) || !(S_IRUSR & fileInfo.st_mode))
    {
        cout << "403 Forbidden: Server couldn't read this file." << endl;
        sendErrorMsg("403", "Forbidden", "Server couldn't read this file");
        return;
    }
    getFileType();
    string msg = "HTTP/1.1 200 OK\r\n";
    msg += "Server: Tiny Web Server\r\n";
    msg += "Content-length: " + to_string(fileInfo.st_size) + "\r\n";
    // msg += "Content-length: " + to_string(static_cast<double>(fileInfo.st_size)) + "\r\n";
    msg += "Content_type: " + _fileType + "\r\n\r\n";
    _outputBuffer.append(msg.c_str(), msg.size());
    int fd = open(_fileName.c_str(), O_RDONLY, 0);
    _outputBuffer.readFd(fd);
    _outputBuffer.sendFd(_connfd);
    close(_connfd);
    _isClosed = true;
}

const int Handler::connFd() const
{
    return _connfd;
}

bool Handler::receiveRequest()
{
    if (_inputBuffer.readFd(_connfd) == 0)
        return false;
    string request = _inputBuffer.readAllAsString();
    cout << "---------------------------Request---------------------------" << endl;
    cout << request << endl;
    cout << "-------------------------------------------------------------" << endl;
    Parser p(request);
    _request = p.getParseResult();
    return true;
}

void Handler::sendErrorMsg(const string &errorNum,
                           const string &shortMsg,
                           const string &longMsg)
{
    string msg = "HTTP /1.0 " + errorNum + " " + shortMsg + "\r\n";
    msg += "Content-type: text/html\r\n";
    msg += "<html><title>Error</title>";
    msg += "<body bgcolor="
           "ffffff"
           ">\r\n";
    msg += errorNum + ": " + shortMsg + "\r\n";
    msg += "<p>" + longMsg + ": " + _fileName + "\r\n";
    msg += "<hr><em>The Tiny Web server</em>\r\n";
    _outputBuffer.append(msg.c_str(), msg.size());
    _outputBuffer.sendFd(_connfd);
    close(_connfd);
    _isClosed = true;
}

void Handler::parseURI()
{
    _fileName = ".";
    if (_request.uri == "/")
        _fileName += "/home.html";
}

void Handler::getFileType()
{
    const char *name = _fileName.c_str();
    if (strstr(name, ".html"))
        _fileType = "text/html";
    else if (strstr(name, ".pdf"))
        _fileType = "application/pdf";
    else if (strstr(name, ".png"))
        _fileType = "image/png";
    else if (strstr(name, ".gif"))
        _fileType = "image/gif";
    else if (strstr(name, ".jpg"))
        _fileType = "image/jpg";
    else if (strstr(name, ".jpeg"))
        _fileType = "image/jpeg";
    else if (strstr(name, ".css"))
        _fileType = "test/css";
    else
        _fileType = "text/plain";
}
