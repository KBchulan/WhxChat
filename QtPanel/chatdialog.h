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

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private:
    void initHead();

private:
    Ui::ChatDialog *ui = nullptr;
};

#endif // CHATDIALOG_H
