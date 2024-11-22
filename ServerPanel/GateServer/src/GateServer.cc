#include "../include/CServer.h"
#include "../include/ConfigManager.h"

int main()
{
    ConfigManager gCfgMgr;
    std::string gate_port_str = gCfgMgr["GateServer"]["port"];
    unsigned short gate_short = atoi(gate_port_str.c_str());

    try
    {
        unsigned short port = static_cast<unsigned short>(14789);
        boost::asio::io_context ioc;

        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&ioc](boost::system::error_code error, auto)
        {
            if (error)
            {
                std::cerr << "Singal receive failed!" << '\n';
                return 0;
            }
            ioc.stop(); 
            return 0;
        });

        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}