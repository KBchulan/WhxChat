#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "const.h"

class HttpConnection final : public std::enable_shared_from_this<HttpConnection>
{
public:
    explicit HttpConnection(boost::asio::ip::tcp::socket socket);

    // 开始监听
    void Start();

private:
    // 检测超时
    void CheckDeadline();

    // 受到数据后产生的应答函数
    void WriteResponse();

    // 处理请求(header, body)
    void HandleReq();

    // 所有的定时器都是绑定在底层的事件循环里，需要绑定一个调度器
    boost::asio::steady_timer _deadline
    {
        _socket.get_executor(),
        std::chrono::seconds(60)
    };

private:
    boost::asio::ip::tcp::socket _socket;
    boost::beast::flat_buffer _buffer{ BUFFER_SIZE };
    boost::beast::http::request<boost::beast::http::dynamic_body> _request;
    boost::beast::http::response<boost::beast::http::dynamic_body> _response;
};

#endif // !HTTPCONNECTION_H
