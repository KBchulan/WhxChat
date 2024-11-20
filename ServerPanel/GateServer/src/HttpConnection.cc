#include "../include/LogicSystem.h"
#include "../include/HttpConnection.h"

#include <iostream>

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket)
    : _socket(std::move(socket))
{
    std::cout << R"(HttpManager has been created!)" << '\n';
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
    auto self = shared_from_this();
    _deadline.async_wait([self](boost::beast::error_code ec)
    {
        if(!ec)
        {
            self->_socket.close(ec);
        }
    });
}

void HttpConnection::WriteResponse()
{
    auto self = shared_from_this();
    _response.content_length(_response.body().size());

    boost::beast::http::async_write(_socket, _response, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        self->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        self->_deadline.cancel();
    });
}

void HttpConnection::HandleReq()
{
    // 设置版本
    _response.version(_request.version());
    _response.keep_alive(false);

    if(_request.method() == boost::beast::http::verb::get)
    {
        bool success = LogicSystem::GetInstance()->HandleGet(_request.target().to_string(), shared_from_this());
        if(!success)
        {
            _response.result(boost::beast::http::status::not_found);
            _response.set(boost::beast::http::field::content_type, "text/plain");
            boost::beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        _response.result(boost::beast::http::status::ok);
        _response.set(boost::beast::http::field::server, "GateServer");
        WriteResponse();
        return;
    }

}
