#include "chatview.h"

#include <QEvent>
#include <QPainter>
#include <QStyleOption>

ChatView::ChatView(QWidget *parent)
    : QWidget(parent), _is_appended(false)
{
    // 创建主垂直布局
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setMargin(0);

    // 创建滚动区域
    _scroll_area = new QScrollArea();
    _scroll_area->setObjectName("chat_area");
    pMainLayout->addWidget(_scroll_area);

    // 创建聊天背景窗口
    QWidget *w = new QWidget(this);
    w->setObjectName("chat_background");
    w->setAutoFillBackground(true);

    // 创建垂直布局并添加一个空白widget作为占位符，将聊天信息放在最上层
    QVBoxLayout *pVLayout_1 = new QVBoxLayout();
    pVLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pVLayout_1);
    _scroll_area->setWidget(w);

    // 设置滚动条，垂直的不要
    _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = _scroll_area->verticalScrollBar();
    
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);

    // 创建水平布局并添加滚动条
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setMargin(0);

    // 设置滚动区域布局
    _scroll_area->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    // 设置滚动区域属性
    _scroll_area->setWidgetResizable(true);
    _scroll_area->installEventFilter(this);

    initStylesheet();
}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(_scroll_area->widget()->layout());
    vl->insertWidget(vl->count() - 1, item);
    _is_appended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{
    
}

bool ChatView::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Enter && watched == _scroll_area)
    {
        _scroll_area->verticalScrollBar()->setHidden(_scroll_area->verticalScrollBar()->maximum() == 0);
    }
    else if(event->type() == QEvent::Leave && watched == _scroll_area)
    {
        _scroll_area->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(watched, event);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatView::initStylesheet()
{

}

void ChatView::onVScrollBarMoved(int, int)
{
    if(_is_appended)
    {
        QScrollBar *pVScrollBar = _scroll_area->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());

        QTimer::singleShot(500, [this]() -> void
        {
            _is_appended = false;
        });
    }
}