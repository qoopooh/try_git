#include <QtGui>

#include "epctreeitem.h"
#include "epctreemodel.h"

EpcTreeModel::EpcTreeModel(const QString &data, QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  rootData << "EPC" << "Count";
  rootItem = new EpcTreeItem(rootData);
  setupModelData(data.split(QString("\n")), rootItem);
}

EpcTreeModel::~EpcTreeModel()
{
  delete rootItem;
}

int EpcTreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<EpcTreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant EpcTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  EpcTreeItem *item = static_cast<EpcTreeItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags EpcTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant EpcTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

QModelIndex EpcTreeModel::index(int row, int column, const QModelIndex &parent)
  const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  EpcTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<EpcTreeItem*>(parent.internalPointer());

  EpcTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex EpcTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  EpcTreeItem *childItem = static_cast<EpcTreeItem*>(index.internalPointer());
  EpcTreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int EpcTreeModel::rowCount(const QModelIndex &parent) const
{
  EpcTreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<EpcTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

void EpcTreeModel::setupModelData(const QStringList &lines, EpcTreeItem *parent)
{
  QList<EpcTreeItem*> parents;
  QList<int> indentations;
  parents << parent;
  indentations << 0;
  int number = 0;
  while (number < lines.count()) {
    int position = 0;
    while (position < lines[number].length()) {
      if (lines[number].mid(position, 1) != " ")
        break;
      position++;
    }
    QString lineData = lines[number].mid(position).trimmed();
    if (!lineData.isEmpty()) {
      // Read the column data from the rest of the line.
      QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
      QList<QVariant> columnData;
      for (int column = 0; column < columnStrings.count(); ++column)
        columnData << columnStrings[column];
      if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.
        if (parents.last()->childCount() > 0) {
          parents << parents.last()->child(parents.last()->childCount()-1);
          indentations << position;
        }
      } else {
        while (position < indentations.last() && parents.count() > 0) {
          parents.pop_back();
          indentations.pop_back();
        }
      }
      // Append a new item to the current parent's list of children.
      parents.last()->appendChild(new EpcTreeItem(columnData, parents.last()));
    }

  number++;
  }
}

void EpcTreeModel::insertEpc(const QString &epc)
{
  bool f_dup_epc = false;
  EpcTreeItem *item, *subitem;

  for (int i = 0; i < rootItem->childCount(); ++i) {
    item = rootItem->child(i);
    if (item->data(0).toString().compare(epc) == 0) {
      int count = item->data(1).toInt();

      item->setData(1, ++count);
      subitem = item->child(1);
      subitem->setData(1, QTime::currentTime().toString("hh:mm:ss.zzz"));
      f_dup_epc = true;
    }
  }

  if (f_dup_epc)
    return;

  QList<QVariant> columnData;
  columnData << epc;
  columnData << 1;
  item = new EpcTreeItem(columnData, rootItem);
  columnData.clear();
  columnData << tr("First read");
  columnData << QDate::currentDate().toString("yyyy-MM-dd ")
                + QTime::currentTime().toString("hh:mm:ss.zzz");
  item->appendChild(new EpcTreeItem(columnData, item));
  columnData.clear();
  columnData << tr("Last read");
  columnData << QTime::currentTime().toString("hh:mm:ss.zzz");
  item->appendChild(new EpcTreeItem(columnData, item));
  rootItem->appendChild(item);
}

