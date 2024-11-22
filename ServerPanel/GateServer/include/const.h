#ifndef CONST_H
#define CONST_H

// 第三方库
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/beast/http.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

// cc
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <condition_variable>

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

class ConfigManager;
extern ConfigManager gCfgMgr;

#endif // !CONST_H