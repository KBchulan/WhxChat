/******************************************************************************
 *
 * @file       chatuserwidget.h
 * @brief      chatUserList的每一个组件
 *
 * @author     KBchulan
 * @date       2024/12/01
 * @history
 *****************************************************************************/
#ifndef CHATUSERWIDGET_H
#define CHATUSERWIDGET_H

#include "listitembase.h"

#include <QWidget>

namespace Ui
{
    class ChatUserWidget;
}

class ChatUserWidget final : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWidget(QWidget *parent = nullptr);
    ~ChatUserWidget();

    QSize sizeHint() const override
    {
        return QSize(340, 70);
    }

    void SetInfo(QString _msg, QString _name, QString _head);

private:
    Ui::ChatUserWidget *ui;

private:
    QString _msg;
    QString _name;
    QString _head;
};

#endif // CHATUSERWIDGET_H
