#include "backgrounddialog.h"

BackgroundDialog::BackgroundDialog(QWidget *parent)
    : QDialog(parent)
    , m_opacity(1.0)
{
}

void BackgroundDialog::setBackground(const QString& imagePath, qreal opacity)
{
    m_imagePath = imagePath;
    m_opacity = qBound(0.0, opacity, 1.0); // 确保透明度在0-1之间
    
    if (!m_imagePath.isEmpty()) {
        m_backgroundImage.load(m_imagePath);
    }
    
    // 触发重绘
    update();
}

void BackgroundDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 如果有背景图片
    if (!m_backgroundImage.isNull()) {
        // 设置透明度
        painter.setOpacity(m_opacity);
        
        // 缩放图片以适应窗口大小
        QPixmap scaled = m_backgroundImage.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        
        // 计算居中位置
        QPoint pos((width() - scaled.width()) / 2, (height() - scaled.height()) / 2);
        
        // 绘制背景
        painter.drawPixmap(pos, scaled);
    }
    
    // 调用基类绘制事件以确保其他内容正常显示
    QDialog::paintEvent(event);
} 