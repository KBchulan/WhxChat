#include "listitembase.h"

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
