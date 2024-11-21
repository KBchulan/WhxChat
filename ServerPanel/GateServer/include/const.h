#ifndef CONST_H
#define CONST_H

// 第三方库
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

// cc
#include <map>
#include <memory>
#include <functional>
#include <unordered_map>

// personal
#include "Singleton.h"

// 常量定义
static constexpr size_t BUFFER_SIZE = 8192;

enum ErrorCodes
{
    Success = 0,
    ErrorJson = 1001,
    RPCFailed = 1002,       // 分布式系统中远程过程调用错误
};

#endif // !CONST_H