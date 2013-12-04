#include "epcdb.h"

EpcDb::EpcDb()
{
  db =  QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("epc.sqlite");

  if (db.open()) {
    QSqlQuery query(db);
    if (query.exec("CREATE TABLE epc (INTEGER id PRIMARY KEY );")) {
      qDebug() << "Create table";
    } else {
      qDebug() << "Database exists! " << query.lastError();
    }
  } else {
    qDebug() << "Cannot open database";
  }
}

EpcDb::~EpcDb()
{
  db.close();
}

bool EpcDb::addEpc(const QByteArray &epchex)
{
  return false;
}

int EpcDb::getEpcCount()
{
  return 0;
}

void EpcDb::clear()
{

}

