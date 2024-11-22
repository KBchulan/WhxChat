#include "../include/LogicSystem.h"
#include "../include/HttpConnection.h"
#include "../include/VerifyGrpcClient.h"

LogicSystem::~LogicSystem()
{
    std::cout << "LogicSystem has been destructed!" << '\n';
}

LogicSystem::LogicSystem()
{
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection)
    {
        boost::beast::ostream(connection->_response.body()) << "receive get_test req" << '\n';
        int i = 0;
        for(auto &elem : connection->_get_params)
        {
            i++;
            boost::beast::ostream(connection->_response.body())
                << "params" << i << ": key is " << elem.first 
                << ", value is " << elem.second << '\n';
        }
    });

    RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection)
    {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is: " << body_str << '\n';

        connection->_response.set(boost::beast::http::field::content_type, "text/json");
        
        Json::Value dst_root;
        Json::Reader reader;
        Json::Value src_root;

        bool parse_success = reader.parse(body_str, src_root);
        if(!parse_success)
        {
            std::cout << "Failed to parse Json data" << '\n';
            dst_root["error"] = ErrorCodes::ErrorJson;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr << '\n'; 
            return true;
        }

        if(src_root.isMember("email"))
        {
            auto email = src_root["email"].asString();
            GetVarifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
            dst_root["error"] = rsp.error();
            dst_root["email"] = src_root["email"];
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr << '\n'; 
            return true;
        }
        else
        {
            std::cout << "Failed to parse Json data" << '\n';
            dst_root["error"] = ErrorCodes::ErrorJson;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr << '\n'; 
            return true;
        }

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

void LogicSystem::RegPost(const std::string &url, HttpHandler handler)
{
    _post_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandlePost(const std::string &path, std::shared_ptr<HttpConnection> con)
{
    if(_post_handlers.find(path) == _post_handlers.end())
    {
        return false;
    }

    _post_handlers[path](con);
    return true;
}
