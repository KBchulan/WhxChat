#include "../include/LogicSystem.h"
#include "../include/RedisManager.h"
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
            std::cerr << "Failed to parse Json data" << '\n';
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
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }
        else
        {
            std::cout << "Failed to parse Json data" << '\n';
            dst_root["error"] = ErrorCodes::ErrorJson;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }
    });

    RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection)
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
            std::cerr << "Failed to parse Json data" << '\n';
            dst_root["error"] = ErrorCodes::ErrorJson;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }

        // 输入密码和确认是否一样
        if(src_root["passwd"].asString() != src_root["confirm"].asString())
        {
            std::cerr << "confirm not equal to passwd" << '\n';
            dst_root["error"] = ErrorCodes::PasswdErr;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }

        std::string varify_code;
        bool b_get_varify = RedisManager::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
        
        // 验证码过期
        if(!b_get_varify)
        {
            std::cerr << "get varify code expired" << '\n';
            dst_root["error"] = ErrorCodes::VarifyExpired;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }

        // 验证码不匹配
        if(varify_code != src_root["varifycode"].asString())
        {
            std::cerr << "varify code error" << '\n';
            dst_root["error"] = ErrorCodes::VarifyCodeErr;
            std::string jsonstr = dst_root.toStyledString();
            boost::beast::ostream(connection->_response.body()) << jsonstr; 
            return true;
        }
        
        // mysql查找user
        
        // 回包
        dst_root["error"] = 0;
        dst_root["email"] = src_root["email"];
        dst_root ["user"]= src_root["user"].asString();
        dst_root["passwd"] = src_root["passwd"].asString();
        dst_root["confirm"] = src_root["confirm"].asString();
        dst_root["varifycode"] = src_root["varifycode"].asString();
        std::string jsonstr = dst_root.toStyledString();
        boost::beast::ostream(connection->_response.body()) << jsonstr;
        return true;
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
