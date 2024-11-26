#include "../include/CServer.h"
#include "../include/LogManager.h"

int main()
{
    if(!LogManager::GetInstance()->Init("../../../logs", LogLevel::DEBUG))
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
                std::cerr << "Signal receive failed!" << std::endl;
                return;
            }
            
            if(!LogManager::GetInstance()->IsShutdown())
            {
                LOG_SERVER->info("Shutting down server");
                LOG_SERVER->flush();
                LogManager::Shutdown();
            }
            ioc.stop();
        });

        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
    }
    catch (const std::exception &e)
    {
        if(!LogManager::GetInstance()->IsShutdown())
        {
            LOG_SERVER->error("Server error: {}", e.what());
            LOG_SERVER->flush();
            LogManager::Shutdown();
        }
        return EXIT_FAILURE;
    }

    return 0;
}