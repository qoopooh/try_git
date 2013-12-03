#include <QStringList>

#include "epctreeitem.h"

EpcTreeItem::EpcTreeItem(const QList<QVariant> &data, EpcTreeItem *parent)
{
  parentItem = parent;
  itemData = data;
}

EpcTreeItem::~EpcTreeItem()
{
  qDeleteAll(childItems);
}
void EpcTreeItem::appendChild(EpcTreeItem *item)
{
  childItems.append(item);
}

EpcTreeItem *EpcTreeItem::child(int row)
{
  return childItems.value(row);
}

int EpcTreeItem::childCount() const
{
  return childItems.count();
}

int EpcTreeItem::columnCount() const
{
  return itemData.count();
}

void EpcTreeItem::setData(int column, QVariant value)
{
  itemData[column] = value;
}

QVariant EpcTreeItem::data(int column) const
{
  return itemData.value(column);
}

EpcTreeItem *EpcTreeItem::parent()
{
  return parentItem;
}

int EpcTreeItem::row() const
{
  if (parentItem)
    return parentItem->childItems.indexOf(const_cast<EpcTreeItem*>(this));

  return 0;
}

