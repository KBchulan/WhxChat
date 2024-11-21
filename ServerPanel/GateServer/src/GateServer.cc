#include "../include/CServer.h"

int main()
{
    try
    {
        unsigned short port = static_cast<unsigned short>(14789);
        boost::asio::io_context ioc;

        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&ioc](auto, auto)
                           { ioc.stop(); });

        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}