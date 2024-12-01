#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include "chatuserwidget.h"

#include <QEvent>
#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <QListWidget>

class ChatUserList final : public QListWidget
{
    Q_OBJECT

public:
    explicit ChatUserList(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
