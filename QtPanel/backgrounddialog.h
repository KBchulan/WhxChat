#ifndef BACKGROUNDDIALOG_H
#define BACKGROUNDDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPixmap>

class BackgroundDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BackgroundDialog(QWidget *parent = nullptr);
    
    // 设置背景图片路径和透明度
    void setBackground(const QString& imagePath, qreal opacity = 1.0);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_imagePath;
    qreal m_opacity;
    QPixmap m_backgroundImage;
};

#endif // BACKGROUNDDIALOG_H 