#ifndef EPCDB_H
#define EPCDB_H

#include "QtSql/QtSql"

const QString k_db_name = "epc.sqlite";

class EpcDb
{
public:
    EpcDb();
    bool addEpc(const QByteArray &epchex, int atten);
    int getEpcCount();
    void clear();
    void report(const QString &path);
    QString error();
    void close();

private:
    QSqlDatabase db;
    bool m_open;
};

#endif // EPCDB_H
