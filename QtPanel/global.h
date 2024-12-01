/******************************************************************************
 *
 * @file       global.h
 * @brief      some headers and global definition
 *
 * @author     KBchulan
 * @date       2024/11/20
 * @history
 *****************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

// qt
#include <QDir>
#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QRegularExpression>

// cc
#include <mutex>
#include <memory>
#include <iostream>
#include <functional>

// personal header
#include "QStyle"

extern QString gate_url_prefix;

// functional
// repolish err_tip(in register_dialog)'s style(in other words, it will repolish qss)
extern std::function<void(QWidget *)> repolish;

// 字符串异或进行加密
extern std::function<QString(QString)> xorString;

enum ReqId
{
    ID_GET_VARIFY_CODE = 1001,  // get varify code(in RegisterDialog)
    ID_REG_USER = 1002,         // register user(in RegisterDialog)
    ID_RESET_PWD = 1003,        // reset password
    ID_LOGIN_USER = 1004,       // login user
    ID_CHAT_LOGIN = 1005,       // login chat server
    ID_CHAT_LOGIN_RSP = 1006,   // when the 1005 be done, the return body
};

enum Modules
{
    REGISTERMOD = 0,            // register mod
    RESETMOD = 1,               // reset password
    LOGINMOD = 2,               // user login
};

enum ErrorCodes
{
    SUCCESS = 0,
    ERR_JSON = 1,               // json parse failed
    ERR_NETWORK = 2,            // all net error
};

enum TipErr
{
    TIP_SUCCESS = 0,            // success
    TIP_EMAIL_ERR = 1,          // not match the regex in register
    TIP_PWD_ERR = 2,            // number, word...
    TIP_PWD_CONFIRM = 3,        // not match
    TIP_VARIFY_ERR = 4,         // just check isNull, the rightness will judge by GateServer
    TIP_USER_ERR = 5,           // some name regex
};

enum ClickLbState
{
    Normal = 0,
    Selected = 1,
};

// 将自己的信息通过网关服务器拿到聊天服务器
struct ServerInfo
{
    int Uid;
    QString Host;
    QString Port;
    QString Token;
};

enum ChatUIMode
{
    SearchMode = 0,      // 搜索模式
    ChatMode = 1,        // 聊天模式
    ContactMode = 2,     // 联系人模式
};

enum ListItemType
{
    CHAT_USER_ITEM = 0,       // 聊天用户
    CONTACT_USER_ITEM = 1,    // 联系人用户
    SEARCH_USER_ITEM = 2,     // 搜索到的用户
    ADD_USER_TIP_ITEM = 3,    // 提示添加用户
    INVALID_ITEM = 4,         // 不可点击条目
    GROUP_TIP_ITEM = 5,       // 分组提示条目
    LINE_ITEM = 6,            // 分割线
    APPLY_FRIEND_ITEM = 7,    // 好友申请
};

#endif // GLOBAL_H
