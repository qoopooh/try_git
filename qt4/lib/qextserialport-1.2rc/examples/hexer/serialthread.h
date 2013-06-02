#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QThread>
#include <QTimer>

#include "qextserialport.h"
#include "qextserialenumerator.h"

class QextSerialPort;
class QextSerialEnumerator;

class SerialThread : public QThread
{
  Q_OBJECT

public:
  SerialThread();
  ~SerialThread();

  static QList<QString> discovery();

public Q_SLOTS:
  void turnOn(bool =true);
  void turnOff(bool off=true);
  void setPort(const QString &);
  void setBaud(const QString &);

signals:
  void data(QString);

protected Q_SLOTS:
  void onReadyRead();

private:
  struct SerialInfo;
  SerialInfo *m_info;
  QTimer *timer;
  QextSerialPort *port;
  QextSerialEnumerator *enumerator;
};

#endif // SERIALTHREAD_H

