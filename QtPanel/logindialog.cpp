#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // 注册按钮
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    // 忘记密码的样式
    ui->forget_label->SetState("normal", "hover", "hover", "selected", "selected_hover", "selected_hover");

    connect(ui->forget_label, &ClickedLabel::sig_clicked, this, &LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    qDebug() << "login destruce";
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    emit switchReset();
}
