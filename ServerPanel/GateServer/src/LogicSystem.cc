#include "../include/LogicSystem.h"
#include "../include/HttpConnection.h"

LogicSystem::~LogicSystem()
{

}

LogicSystem::LogicSystem()
{
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection)
    {
        boost::beast::ostream(connection->_response.body()) << "receive get_test req";
    });
}

void LogicSystem::RegGet(const std::string &url, HttpHandler handler)
{
    _get_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandleGet(const std::string &path, std::shared_ptr<HttpConnection> con)
{
    if(_get_handlers.find(path) == _get_handlers.end())
    {
        return false;
    }

    _get_handlers[path](con);
    return true;
}

