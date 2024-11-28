#ifndef MYSQLDAO_H
#define MYSQLDAO_H

#include "const.h"

class SqlConnection
{
public:
    SqlConnection(sql::Connection *con, std::uint64_t lastTime);

public:
    // 用于超时检测，记录上一次的sql语句执行的时间
    std::uint64_t _last_oper_time;
    std::unique_ptr<sql::Connection> _con;
};

class MysqlPool
{
public:
    // 构造函数，初始化连接池
    explicit MysqlPool(const std::string &url, const std::string &user, const std::string &passwd, const std::string &schema, std::uint32_t poolSize);

    // 析构函数，释放连接池资源
    ~MysqlPool();

    // 检查连接是否有效，发送心跳包
    void CheckConnection();

    // 获取一个数据库连接
    std::unique_ptr<SqlConnection> GetConnection();

    // 归还一个数据库连接到连接池
    void ReturnConnection(std::unique_ptr<SqlConnection> con);

    // 关闭连接池
    void Close();

private:
    // 数据库属性
    std::string _url;
    std::string _user;
    std::string _passwd;
    std::string _schema;

    // pool属性
    std::uint32_t _poolSize;
    std::atomic_bool _b_stop;
    std::queue<std::unique_ptr<SqlConnection>> _pool;

    // 线程
    std::mutex _mutex;
    std::thread _check_thread;          // 发送心跳包
    std::condition_variable _cond;
};

struct UserInfo
{
    int _uid;
    std::string _name;
    std::string _email;
    std::string _passwd;
};

class MysqlDao
{
public:
    MysqlDao();
    ~MysqlDao();

    // 注册用户,返回错误码
    int RegUser(const std::string &name, const std::string &email, const std::string &passwd);

    // 使用事务注册用户,包含头像上传,返回错误码
    // int RegUserTransaction(const std::string &name, const std::string &email, const std::string &passwd, const std::string &icon);
    
    // 检查用户名和邮箱是否匹配
    bool CheckEmail(const std::string &name, const std::string &email);
    
    // 更新用户密码
    bool UpdatePasswd(const std::string &name, const std::string &newpasswd);
    
    // 验证用户密码,成功则返回用户信息
    // bool CheckPasswd(const std::string &name, const std::string &passwd, UserInfo &userInfo);
    
    // 测试存储过程
    // bool TestProcedure(const std::string &email, int &uid, std::string &name);

private:
    std::unique_ptr<MysqlPool> _pool;
};

#endif // !MYSQLDAO_H