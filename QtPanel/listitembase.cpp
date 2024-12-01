#include "listitembase.h"

#include <QPainter>
#include <QStyleOption>

ListItemBase::ListItemBase(QWidget *parent)
    : QWidget(parent)
{

}

void ListItemBase::SetItemType(ListItemType type)
{
    _type = type;
}

ListItemType ListItemBase::GetItemType()
{
    return _type;
}

void ListItemBase::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
