#include "../include/CServer.h"
#include "../include/HttpConnection.h"

#include <iostream>

CServer::CServer(boost::asio::io_context &ioc, unsigned short port) 
    : _ioc(ioc), _socket(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    std::cout << "GateServer start on port :" << port << '\n';
}

void CServer::Start()
{
    auto self = shared_from_this();
    _acceptor.async_accept(_socket, [self](boost::system::error_code ec)
    {
        try
        {
            // 如果出错就放弃这个连接，去监听其他的连接
            if(ec)
            {
                self->Start();
                return;
            }   

            // 创建新连接，并且创建一个httpConnection管理这个连接
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

            self->Start();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    });
}