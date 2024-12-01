#include "chatuserwidget.h"
#include "ui_chatuserwidget.h"

ChatUserWidget::ChatUserWidget(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ChatUserWidget)
{
    ui->setupUi(this);

    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWidget::~ChatUserWidget()
{
    delete ui;
}

void ChatUserWidget::SetInfo(QString msg, QString name, QString head)
{
    _msg = msg;
    _name = name;
    _head = head;

    QPixmap pixmap(_head);
    ui->icon_label->setPixmap(pixmap.scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_label->setScaledContents(true);

    ui->user_name_label->setText(_name);
    ui->user_chat_label->setText(_msg);
}
