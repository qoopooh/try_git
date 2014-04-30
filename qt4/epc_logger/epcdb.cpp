#include "epcdb.h"

EpcDb::EpcDb()
{
  qDebug()<<"QSqlDatabase drivers: "<< QSqlDatabase::drivers();
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(k_db_name);

  m_open = db.open();
  if (m_open) {
    QSqlQuery query("CREATE TABLE \"epc\" (\"id\" VARCHAR PRIMARY KEY, \
                    \"time\" DATETIME, \"attenuation\" INTEGER);", db);
    if (query.exec()) {
      qDebug() << "Create table";
    } else {
      qDebug() << "Database exists! " << query.lastError();
    }
  } else {
    qDebug() << "Cannot open database";
  }
  db.close();
}

bool EpcDb::addEpc(const QByteArray &epchex, int atten)
{
  bool success = false;

  if (!m_open)
    return false;
  db.open();
  QSqlQuery query("SELECT id FROM epc WHERE id='" + epchex + "'", db);
  while (query.next()) {
    db.close();
    return true;
  }

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

  if (!m_open)
    return -1;
  db.open();
  QSqlQuery query("SELECT id FROM epc", db);
  while (query.next()) {
    ++number;
  }
  db.close();

  return number;
}

void EpcDb::clear()
{
  db.open();
  QSqlQuery query("DELETE FROM epc", db);
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
  QSqlQuery query("SELECT * FROM epc", db);
  while (query.next()) {
    ++number;
    stream << "'" << query.value(0).toString() << "," << query.value(1).toString()
           << "," << query.value(2).toString() << endl;
  }
  qDebug() << "count " << number;
  file.close();
  db.close();
}

QString EpcDb::error()
{
  return db.lastError().text();
}

void EpcDb::close()
{
  QString connection;
  connection=db.connectionName();

  db = QSqlDatabase();
  db.removeDatabase(connection);
  qDebug() << "close db";
}
