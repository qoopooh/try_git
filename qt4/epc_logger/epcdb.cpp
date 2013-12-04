#include "epcdb.h"

EpcDb::EpcDb()
{
  db =  QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("epc.sqlite");

  if (db.open()) {
    QSqlQuery query(db);
    if (query.exec("CREATE TABLE \"epc\" (\"id\" VARCHAR PRIMARY KEY, \
                   \"time\" DATETIME, \"attenuation\" INTEGER);")) {
      qDebug() << "Create table";
    } else {
      qDebug() << "Database exists! " << query.lastError();
    }
    db.close();
  } else {
    qDebug() << "Cannot open database";
  }
}

EpcDb::~EpcDb()
{
  db.close();
}

bool EpcDb::addEpc(const QByteArray &epchex, int atten)
{
  bool exist = false;
  bool success = false;

  db.open();
  QSqlQuery query("SELECT id FROM epc WHERE id='" + epchex + "'");
  while (query.next()) {
    exist = true;
  }
  if (exist)
    return false;

  query.prepare("INSERT INTO epc (id, time, attenuation) VALUES (?, ?, ?)");
  query.addBindValue(QString(epchex));
  QDateTime date = QDateTime::currentDateTime();
  QString dateString = date.toString("yyyy-MM-dd hh:mm:ss");
  query.addBindValue(dateString);
  query.addBindValue(atten);
  success = query.exec();
  db.close();

  return success;
}

int EpcDb::getEpcCount()
{
  int number = 0;

  db.open();
  QSqlQuery query("SELECT id FROM epc");
  while (query.next()) {
    ++number;
  }
  db.close();

  return number;
}

void EpcDb::clear()
{
  db.open();
  QSqlQuery query("DELETE FROM epc");
  query.exec();
  db.close();
}

void EpcDb::report(const QString &path)
{
  int number = 0;

  qDebug() << "File name: " << path;
  QFile file(path);
  file.open(QIODevice::WriteOnly);
  QTextStream stream(&file);
  db.open();
  QSqlQuery query("SELECT * FROM epc");
  while (query.next()) {
    ++number;
    stream << "'" << query.value(0).toString() << "," << query.value(1).toString()
           << "," << query.value(2).toString() << endl;
  }
  qDebug() << "count " << number;
  file.close();
  db.close();
}
