#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "const.h"

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// 日志级别
enum class LogLevel
{
    TRACE = SPDLOG_LEVEL_TRACE,
    DEBUG = SPDLOG_LEVEL_DEBUG,
    INFO = SPDLOG_LEVEL_INFO,
    WARN = SPDLOG_LEVEL_WARN,
    ERROR = SPDLOG_LEVEL_ERROR,
    CRITICAL = SPDLOG_LEVEL_CRITICAL,
    OFF = SPDLOG_LEVEL_OFF
};

class LogManager final : public Singleton<LogManager>
{
    friend class Singleton<LogManager>;

public:
    ~LogManager();

    // 初始化日志系统
    bool Init(const std::string &log_path = "logs", LogLevel level = LogLevel::DEBUG);

    // 获取不同类型的logger
    std::shared_ptr<spdlog::logger> GetServerLogger() { return _server_logger; }
    std::shared_ptr<spdlog::logger> GetHttpLogger() { return _http_logger; }
    std::shared_ptr<spdlog::logger> GetRpcLogger() { return _rpc_logger; }
    std::shared_ptr<spdlog::logger> GetSqlLogger() { return _sql_logger; }

    // 设置日志级别
    void SetLevel(LogLevel level);

    // 刷新所有日志
    void Flush();

private:
    LogManager() = default;

    // 创建logger
    std::shared_ptr<spdlog::logger> CreateLogger(const std::string &logger_name,
                                                 const std::string &file_name);

private:
    std::string _log_path;
    std::shared_ptr<spdlog::logger> _server_logger;
    std::shared_ptr<spdlog::logger> _http_logger;
    std::shared_ptr<spdlog::logger> _rpc_logger;
    std::shared_ptr<spdlog::logger> _sql_logger;
};

// 定义宏简化日志调用
#define LOG_RPC LogManager::GetInstance()->GetRpcLogger()
#define LOG_SQL LogManager::GetInstance()->GetSqlLogger()
#define LOG_HTTP LogManager::GetInstance()->GetHttpLogger()
#define LOG_SERVER LogManager::GetInstance()->GetServerLogger()

// 定义格式化宏
#define LOG_FORMAT(logger, level, ...) \
    logger->level("[{}:{}] {}", __FILE__, __LINE__, fmt::format(__VA_ARGS__))

#endif // LOGMANAGER_H