/******************************************************************************
 *
 * @file       logindialog.h
 * @brief      LoginWindow
 *
 * @author     KBchulan
 * @date       2024/11/19
 * @history
 *****************************************************************************/
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui
{
    class LoginDialog;
}

class LoginDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

signals:
    // change to register
    void switchRegister();

};

#endif // LOGINDIALOG_H
