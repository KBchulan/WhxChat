#include "../include/MysqlDao.h"

SqlConnection::SqlConnection(sql::Connection *con, std::uint64_t lastTime)
    : _con(con), _last_oper_time(lastTime)
{

}