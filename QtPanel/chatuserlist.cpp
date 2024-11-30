#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget *parent)
    : QListWidget(parent)
{
    Q_UNUSED(parent);

    // 设置滚动条不可见
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->viewport())
    {
        if (event->type() == QEvent::Enter)
        {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if (event->type() == QEvent::Leave)
        {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检测滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15;
        
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 是否到达底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();

        if (currentValue >= maxScrollValue)
        {
            qDebug() << "loading chat user";
            emit sig_loading_chat_user();
        }

        return true;
    }

    return QListWidget::eventFilter(watched, event);
}
