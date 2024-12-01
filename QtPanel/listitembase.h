#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include "global.h"

#include <QWidget>

class ListItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit ListItemBase(QWidget *parent = nullptr);

    void SetItemType(ListItemType type);

    ListItemType GetItemType();

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    ListItemType _type;

};

#endif // LISTITEMBASE_H
