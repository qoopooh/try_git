#ifndef EPCDB_H
#define EPCDB_H

#include "QtSql/QtSql"

class EpcDb
{
public:
    EpcDb();
    ~EpcDb();
    bool addEpc(const QByteArray &epchex, int atten);
    int getEpcCount();
    void clear();
    void report(const QString &path);

private:
    QSqlDatabase db;
};

#endif // EPCDB_H
