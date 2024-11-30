#include "clickedbtn.h"
#include "chatdialog.h"
#include "ui_chatdialog.h"

#include <QDebug>
#include <QPainter>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    ui->add_btn->SetState("normal", "hover", "press");
    
    // 初始化头像
    initHead();
}

ChatDialog::~ChatDialog()
{
    qDebug() << "chatDialog destructed!";
    delete ui;
}

void ChatDialog::initHead()
{
    QPixmap pixmap(":/resources/Chat/left/head.jpg");

    // 纵横比缩放
    pixmap = pixmap.scaled(ui->side_head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->side_head_label->setPixmap(pixmap);
}
