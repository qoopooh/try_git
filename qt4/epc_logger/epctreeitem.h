#ifndef EPCTREEITEM_H
#define EPCTREEITEM_H

#include <QList>
#include <QVariant>

class EpcTreeItem
{
public:
    EpcTreeItem(const QList<QVariant> &data, EpcTreeItem *parent = 0);
    ~EpcTreeItem();

    void appendChild(EpcTreeItem *child);

    EpcTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    void setData(int column, QVariant value);
    QVariant data(int column) const;
    int row() const;
    EpcTreeItem *parent();

private:
    QList<EpcTreeItem*> childItems;
    QList<QVariant> itemData;
    EpcTreeItem *parentItem;
};

#endif // EPCTREEITEM_H

