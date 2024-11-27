#include "../include/RedisManager.h"
#include "../include/ConfigManager.h"

RedisConnectPool::RedisConnectPool(std::uint32_t size, std::string host, std::uint16_t port, const char *password)
    : _poolSize(size), _host(host), _port(port), _b_stop(false)
{
    for(size_t i = 0; i < _poolSize; i++)
    {
        auto *context = redisConnect(_host.c_str(), _port);

        // connect failed
        if(context == nullptr || context->err != 0)
        {
            if(context != nullptr)
                redisFree(context);
            continue;
        }

        auto reply = (redisReply *)redisCommand(context, "AUTH %s", password);

        // auth failed
        if(reply == nullptr)
        {
            std::cerr << "认证失败" << '\n';
            redisFree(context);
            continue;
        }
        if(reply->type == REDIS_REPLY_ERROR)
        {
            std::cerr << "认证失败" << '\n';
            redisFree(context);
            freeReplyObject(reply); 
            continue;
        }

        freeReplyObject(reply);
        _connections.push(std::move(context));
    }
}

RedisConnectPool::~RedisConnectPool()
{
    std::lock_guard<std::mutex> lock(_mutex);
    while(!_connections.empty())
    {
        auto* conn = _connections.front();
        redisFree(conn);
        _connections.pop();
    }
}

redisContext *RedisConnectPool::GetRedisConnection()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this]()
    {
        if(_b_stop)
            return true;
        return !_connections.empty();
    });

    auto *context = std::move(_connections.front());
    _connections.pop();
    return context;
}

void RedisConnectPool::returnRedisConnection(redisContext *context)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if(_b_stop)
        return;
    
    _connections.push(std::move(context));
    _cond.notify_one();
}

void RedisConnectPool::Close()
{
    _b_stop = true;
    _cond.notify_all();
}

RedisManager::RedisManager()
{
    auto gCfgMgr = ConfigManager::GetInstance();
    std::string host = gCfgMgr["Redis"]["host"];
    std::string port = gCfgMgr["Redis"]["port"];
    std::string password = gCfgMgr["Redis"]["password"];
    _con_pool.reset(new RedisConnectPool(std::thread::hardware_concurrency(), host, static_cast<std::uint16_t>(atoi(port.c_str())), password.c_str()));
}

RedisManager::~RedisManager()
{
    Close();
}

bool RedisManager::Get(const std::string &key, std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;
    
    ConnectionGuard guard(_con_pool.get(), connect);

    auto reply = (redisReply *)redisCommand(connect, "GET %s", key.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command, [ Get " << key << " ] falied!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_STRING)
    {
        std::cerr << "Execute command, [ Get " << key << " ] falied!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "Execute command, [ Get " << key << " ] success!" << '\n';
    return true; 
}

bool RedisManager::Set(const std::string &key, const std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;
    
    ConnectionGuard guard(_con_pool.get(), connect);

   auto reply = (redisReply *)redisCommand(connect, "SET %s %s", key.c_str(), value.c_str());

   if(reply == nullptr)
    {
        std::cerr << "Execute command [ SET " << key << " " << value << " ] failed!" << '\n';
        return false; 
    }

    if(!(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0))
    {
        std::cerr << "Execute command [ SET " << key << " " << value << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false; 
    }

    freeReplyObject(reply);
    std::cout << "Execute command [ SET " << key << " " << value << " ] success!" << '\n';
    return true;
}

bool RedisManager::Auth(const std::string &password)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "AUTH %s", password.c_str());


    if(reply == nullptr)
    {
        std::cerr << "Password error" << std::endl;
        return false;
    }

    if(reply->type == REDIS_REPLY_ERROR)
    {
        std::cerr << "Password error" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    std::cout << "Password right" << std::endl;
    return true;
}

bool RedisManager::LPush(const std::string &key, const std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "LPUSH %s %s", key.c_str(), value.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command [ LPUSH " << key << " " << value << " ] failed!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0)
    {
        std::cerr << "Execute command [ LPUSH " << key << " " << value << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    std::cout << "Execute command [ LPUSH " << key << " " << value << " ] success!" << '\n';
    return true;
}

bool RedisManager::LPop(const std::string &key, std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "LPOP %s", key.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command [ LPOP " << key << " ] failed!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_STRING)
    {
        std::cerr << "Execute command [ LPOP " << key << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "Execute command [ LPOP " << key << " ] success!" << '\n';
    return true; 
}

bool RedisManager::RPush(const std::string &key, const std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "RPUSH %s %s", key.c_str(), value.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command [ RPUSH " << key << " " << value << " ] failed!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0)
    {
        std::cerr << "Execute command [ RPUSH " << key << " " << value << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    std::cout << "Execute command [ RPUSH " << key << " " << value << " ] success!" << '\n';
    return true;
}

bool RedisManager::RPop(const std::string &key, std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "RPOP %s", key.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command [ RPOP " << key << " ] failed!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_STRING)
    {
        std::cerr << "Execute command [ RPOP " << key << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "Execute command [ RPOP " << key << " ] success!" << '\n';
    return true;
}

bool RedisManager::HSet(const std::string &key, const std::string &hkey, const std::string &value)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Execute command [ HSET " << key << " " << hkey << " " << value << " ] failed!" << '\n';
        return false;
    }

    if(reply->type != REDIS_REPLY_INTEGER)
    {
        std::cerr << "Execute command [ HSET " << key << " " << hkey << " " << value << " ] failed!" << '\n';
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);
    std::cout << "Execute command [ HSET " << key << " " << hkey << " " << value << " ] success!" << '\n';
    return true;
}

bool RedisManager::HSet(const char *key, const char *hkey, const char *hvalue, size_t hvaluelen)
{
    const char *argv[4];
    size_t argvlen[4];
    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvaluelen;

    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommandArgv(connect, 4, argv, argvlen);

    if (reply == nullptr)
    {
        std::cerr << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
        return false;
    }

    if (reply->type != REDIS_REPLY_INTEGER)
    {
        std::cerr << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

std::string RedisManager::HGet(const std::string &key, const std::string &hkey)
{
    const char *argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();

    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return "";

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommandArgv(connect, 3, argv, argvlen);

    if (reply == nullptr)
    {
        std::cerr << "Execut command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
        return "";
    }

    if (reply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(reply);
        std::cerr << "Execut command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
        return "";
    }

    std::string value = reply->str;
    freeReplyObject(reply);
    std::cout << "Execut command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
    return value;
}

bool RedisManager::Del(const std::string &key)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "DEL %s", key.c_str());

    if (reply == nullptr)
    {
        std::cerr << "Execut command [ Del " << key << " ] failure ! " << std::endl;
        return false;
    }

    if (reply->type != REDIS_REPLY_INTEGER)
    {
        std::cerr << "Execut command [ Del " << key << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    std::cout << "Execut command [ Del " << key << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::ExistsKey(const std::string &key)
{
    auto connect = _con_pool->GetRedisConnection();
    if(connect == nullptr)
        return false;

    ConnectionGuard guard(_con_pool.get(), connect);
    
    auto reply = (redisReply *)redisCommand(connect, "exists %s", key.c_str());

    if(reply == nullptr)
    {
        std::cerr << "Not Found [ Key " << key << " ]  ! " << std::endl;
        return false;
    }

    if(reply->type != REDIS_REPLY_INTEGER || reply->integer == 0)
    {
        std::cerr << "Not Found [ Key " << key << " ]  ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    
    std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

void RedisManager::Close()
{
    _con_pool->Close();
}
