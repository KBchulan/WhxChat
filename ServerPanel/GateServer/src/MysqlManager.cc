#include "../include/MysqlManager.h"

MysqlManager::~MysqlManager()
{
    
}

MysqlManager::MysqlManager()
{

}

int MysqlManager::RegUser(const std::string &name, const std::string &email, const std::string &pwd)
{
    return _dao.RegUser(name, email, pwd);
}