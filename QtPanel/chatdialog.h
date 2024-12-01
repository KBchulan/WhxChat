/******************************************************************************
 *
 * @file       chatdialog.h
 * @brief      主要的聊天界面
 *
 * @author     KBchulan
 * @date       2024/11/30
 * @history
 *****************************************************************************/
#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "global.h"
#include "chatuserwidget.h"

#include <QDialog>

namespace Ui 
{
    class ChatDialog;
}

class ChatDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    void addChatUserList();

private:
    // 初始化头像
    void initHead();

    // 显示搜索框
    void ShowSearch(bool bSearch = false);

private:
    Ui::ChatDialog *ui = nullptr;
    
    bool _b_loading;         // 是否正在加载
    ChatUIMode _mode;        // 当前模式(side_bar or mid_bar)
    ChatUIMode _state;       // 当前状态

private slots:
    void slot_loading_chat_user();
};

#endif // CHATDIALOG_H
