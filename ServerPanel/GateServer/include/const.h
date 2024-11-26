#ifndef CONST_H
#define CONST_H

// 第三方库
// hiredis
#include <hiredis/hiredis.h>

// boost
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/beast/http.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

// jsoncpp
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

// mysql-connector
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

// cc
#include <map>
#include <mutex>
#include <queue>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include <condition_variable>

// personal
#include "Singleton.h"

// 常量定义
static constexpr size_t BUFFER_SIZE = 8192;
#define CODEPREFIX "code_"

enum ErrorCodes
{
    Success = 0,
    ErrorJson = 1001,
    RPCFailed = 1002,
    VarifyExpired = 1003,
    VarifyCodeErr = 1004,
    UserExist = 1005,
    PasswdErr = 1006,
    EmailNotMatch = 1007,
    PasswdUpFailed = 1008,
    PasswdInvalid = 1009,
};

// RAII统一实现，仿go语言的defer
class Defer
{
public:
    Defer(std::function<void()> func) : _func(func) {}

    ~Defer()
    {
        _func();
    }

private:
    std::function<void()> _func;
};

#endif // !CONST_H