#include "../include/LogManager.h"
#include "../include/RedisManager.h"
#include "../include/ConfigManager.h"
#include "../include/StatusServiceImpl.h"

StatusServiceImpl::StatusServiceImpl()
{
    auto &cfg = ConfigManager::GetInstance();
    auto server_lists = cfg["Chatservers"]["name"];

    std::vector<std::string> words;

    std::stringstream ss(server_lists);
    std::string word;

    while(std::getline(ss, word, ','))
        words.push_back(word);
    
    for (auto &wor : words)
    {
        if(cfg[wor]["name"].empty())
        {
            LOG_SERVER->error("Chat server {} not found in config", wor);
            continue;
        }
        
        ChatServer server;
        server.port = cfg[wor]["port"];
        server.host = cfg[wor]["host"];
        server.name = cfg[wor]["name"];
        _servers[server.name] = server;
    }
}

std::string generate_unique_string()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    return boost::uuids::to_string(uuid);
}

Status StatusServiceImpl::GetChatServer(ServerContext *context, const GetChatServerReq *request, GetChatServerRsp *reply)
{
    LOG_SERVER->info("Received GetChatServer request from uid: {}", request->uid());

    const auto &server = getChatServer();
    
    LOG_SERVER->info("Selected chat server: {} with host: {} and port: {}", server.name, server.host, server.port);

    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ErrorCodes::Success);
    reply->set_token(generate_unique_string());
    insertToken(request->uid(), reply->token());

    LOG_SERVER->info("Generated token: {} for uid: {}", reply->token(), request->uid());

    return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext *context, const LoginReq *request, LoginRsp *reply)
{
    auto uid = request->uid();
	auto token = request->token();

	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";

	bool success = RedisManager::GetInstance()->Get(token_key, token_value);
	if (success) 
    {
		reply->set_error(ErrorCodes::UidInvalid);
		return Status::OK;
	}
	
	if (token_value != token) 
    {
		reply->set_error(ErrorCodes::TokenInvalid);
		return Status::OK;
	}

	reply->set_error(ErrorCodes::Success);
	reply->set_uid(uid);
	reply->set_token(token);
	return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
    std::lock_guard<std::mutex> lock(_server_mtx);
    _tokens[uid] = token;
}

ChatServer StatusServiceImpl::getChatServer()
{
    std::lock_guard<std::mutex> lock(_server_mtx);

    LOG_SERVER->info("Getting chat server with {} servers", _servers.size());

    auto minServer = _servers.begin()->second;

    for (const auto &server: _servers)
    {
        if(server.second.con_count < minServer.con_count)
            minServer = server.second;
    }
	return minServer;
}
