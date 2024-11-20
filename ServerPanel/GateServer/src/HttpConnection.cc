#include "../include/HttpConnection.h"

#include <iostream>

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket)
    : _socket(std::move(socket))
{

}

void HttpConnection::Start()
{
    auto self = shared_from_this();
    boost::beast::http::async_read(_socket, _buffer, _request, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        try
        {
            if(ec)
            {
                std::cerr << R"(Http read err is: )" << ec.message() << '\n';
                return;
            }

            boost::ignore_unused(bytes_transferred);
            self->HandleReq();
            self->CheckDeadline();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    });
}

void HttpConnection::CheckDeadline()
{
    
}

void HttpConnection::WriteResponse()
{

}

void HttpConnection::HandleReq()
{

}
