#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QTimer>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>

class ChatView final : public QWidget
{
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = nullptr);

    // 尾插气泡
    void appendChatItem(QWidget *item);

    // 头插气泡
    void prependChatItem(QWidget *item);

    // 中间插入
    void insertChatItem(QWidget *before, QWidget *item);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void initStylesheet();

private:
    bool _is_appended;
    QVBoxLayout *_pvl;           // 主垂直布局
    QScrollArea *_scroll_area;   // 当前显示的区域,滚动布局

private slots:
    void onVScrollBarMoved(int min, int max);
};

#endif // CHATVIEW_H
