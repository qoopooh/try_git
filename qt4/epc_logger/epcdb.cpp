#include "epcdb.h"

EpcDb::EpcDb()
{
  db =  QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("epc.sqlite");
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

