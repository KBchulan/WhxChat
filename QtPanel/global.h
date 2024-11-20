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
#include <QWidget>
#include <QByteArray>
#include <QNetworkReply>
#include <QRegularExpression>

// cc
#include <mutex>
#include <memory>
#include <iostream>
#include <functional>

// personal header
#include "QStyle"

// functional
// repolish err_tip(in register_dialog)'s style(in other words, it will repolish qss)
extern std::function<void(QWidget *)> repolish;

enum ReqId
{
    ID_GET_VARIFY_CODE = 1001,  // get varify code(in RegisterDialog)
    ID_REG_USER = 1002,         // register user(in RegisterDialog)
};

enum Modules
{
    REGISTERMOD = 0,            // register mod
};

enum ErrorCodes
{
    SUCCESS = 0,
    ERR_JSON = 1,               // json parse failed
    ERR_NETWORK = 2,            // all net error
};

#endif // GLOBAL_H
