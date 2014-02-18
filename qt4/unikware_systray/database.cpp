#include "database.h"
#include <QDebug>

const QString CLIENT_EXIST("SELECT id FROM run WHERE name='%1'");
const QString FIRST_LOGIN("INSERT INTO run (name, login, logout) VALUES ('%1', '%2', '1970-01-01')");
const QString LOGIN("UPDATE run SET login='%2' WHERE name='%1'");
const QString LOGOUT("UPDATE run SET logout='%2' WHERE name='%1'");

Database::Database(QObject *parent) : QObject(parent),
  host("192.168.1.15"), dbname("unikware"), user("berm"), pass("berm")
{
  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName(host);
  db.setDatabaseName(dbname);
  db.setUserName(user);
  db.setPassword(pass);
}

bool Database::login(const QString &client)
{
  QString q;
  if (isExist(client)) {
    q = LOGIN;
  } else {
    q = FIRST_LOGIN;
  }
  q = q.arg(
        client,
        time.currentDateTime().toString("yyyy-MM-dd hh.mm.ss"));

  bool ok = db.open();
  if (!ok) {
    emit error(CannotOpen, tr("Cannot open db"));
    return false;
  }

  QSqlQuery query;
  ok = query.exec(q);
  qDebug() << "login" << query.executedQuery() << ok;
  db.close();

  return ok;
}

bool Database::logout(const QString &client)
{
  bool ok = db.open();
  if (!ok) {
    emit error(CannotOpen, tr("Cannot open db"));
    return false;
  }

  QString q(LOGOUT.arg(
              client,
              time.currentDateTime().toString("yyyy-MM-dd hh.mm.ss")));
  QSqlQuery query;
  ok = query.exec(q);
  qDebug() << "logout" << query.executedQuery() << ok;
  db.close();

  return ok;
}

bool Database::isAdmin(const QString &usr, const QString &passwd)
{
  QCryptographicHash hash(QCryptographicHash::Sha1);
  hash.addData(passwd.toUtf8());
  qDebug() << "usr" << usr
           << "passwd" << passwd
           << "sha1" << hash.result();
  return false;
}

bool Database::isExist(const QString &client)
{
  bool ok = db.open();

  if (!ok) {
    qDebug() << "Cannot open db";
    return false;
  }

  ok = false;
  QString q(CLIENT_EXIST.arg(client));
  QSqlQuery query(q);
  if (query.next())
    ok = true;
  db.close();
  qDebug() << "isExist" << query.executedQuery() << ok;
  return ok;
}

