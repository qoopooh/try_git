#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QCryptographicHash>

class Database : public QObject
{
  Q_OBJECT
public:
  typedef enum {
    CannotOpen,
    CannotSelect,
    CannotInsert,
    WrongFormat,
    ErrorEtc
  } Error;
  explicit Database(QObject *parent = 0);

  bool login(const QString &client);
  bool logout(const QString &client);

  bool isAdmin(const QString &usr, const QString &passwd);

signals:
  void error(int, QString);

public slots:

private:
  bool isExist(const QString &client);

  QSqlDatabase db;
  QString host;
  QString dbname;
  QString user;
  QString pass;
  QDateTime time;
};

#endif // DATABASE_H
