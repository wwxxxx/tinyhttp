#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <string.h>
using std::cout;
using std::endl;
using std::find_if;
using std::string;
using std::vector;

//解析请求后的数据存储在http_request结构体中
typedef struct
{
    string method;     // 请求的方法
    string uri;        // 请求的uri
    string version;    // HTTP版本
    string host;       // 请求的主机名
    string connection; // Connection首部
} HTTPRequest;

class Parser
{
  public:
    Parser(const string request);
    HTTPRequest getParseResult();

  private:
    void parseLine();        // 将请求按行解析存入_lines数组中
    void parseRequestLine(); // 解析请求行
    void parseHeaders();     // 解析头部字段
    string _request;    // 客户的原始请求
    vector<string> _lines;
    HTTPRequest _parseResult;
};

#endif