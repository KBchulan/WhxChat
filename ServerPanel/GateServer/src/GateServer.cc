#include "../include/CServer.h"
#include "../include/LogManager.h"

int main()
{
    if(!LogManager::GetInstance()->Init("logs", LogLevel::DEBUG))
    {
        std::cerr << "初始化日志系统失败" << std::endl;
        return -1;
    }
    
    LOG_SERVER->info("服务器启动中");

    try
    {
        unsigned short port = static_cast<unsigned short>(14789);
        boost::asio::io_context ioc;

        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&ioc](boost::system::error_code error, auto)
        {
            if (error)
            {
                LOG_SERVER->error("Signal receive failed!");
                return;
            }
            LOG_SERVER->info("Shutting down server");
            // 确保日志写入
            LogManager::GetInstance()->Flush();
            ioc.stop();
        });

        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
        
        // 程序结束前再次确保日志写入
        LOG_SERVER->info("Server shutdown complete");
        LogManager::GetInstance()->Flush();
    }
    catch (const std::exception &e)
    {
        LOG_SERVER->error("Server error: {}", e.what());
        LogManager::GetInstance()->Flush();
        return EXIT_FAILURE;
    }

    return 0;
}