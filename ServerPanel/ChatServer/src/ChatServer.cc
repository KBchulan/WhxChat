#include "../include/LogManager.h"
#include "../include/AsioIOContextPool.h"

int main()
{
    if(!LogManager::GetInstance()->Init("../../../logs", LogLevel::DEBUG))
    {
        std::cerr << "初始化日志系统失败" << std::endl;
        return EXIT_FAILURE;
    }

    LOG_SERVER->info(R"(GateServer start!)");

    try
    {
        auto &pool = AsioIOContextPool::GetInstance();

        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&pool, &ioc](auto, auto)
        {
        });

        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}