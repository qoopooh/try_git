#ifndef EPCTREEMODEL_H
#define EPCTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class EpcTreeItem;

class EpcTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EpcTreeModel(const QString &data, QObject *parent = 0);
    ~EpcTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void insertEpc(const QString &epc);
    int count();

signals:

public slots:

private:
    void setupModelData(const QStringList &lines, EpcTreeItem *parent);

    EpcTreeItem *rootItem;
};

#endif // EPCTREEMODEL_H
