#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QTimer>
#include <QPushButton>

class TimerBtn : public QPushButton
{
public:
    TimerBtn(QWidget *parent = nullptr);
    ~TimerBtn();

    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    int _counter;
    QTimer *_timer;
};

#endif // TIMERBTN_H
