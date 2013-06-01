#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QThread>

#include "qextserialport.h"
#include "qextserialenumerator.h"

class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class SerialThread : public QThread
{
  Q_OBJECT

public:
  SerialThread(QObject *parent = 0);
  ~SerialThread();

  static QList<QString> discovery();

public Q_SLOTS:
  void turnOn(bool on=true);
  void turnOff(bool off=true);

signals:
  void data(QString);

protected Q_SLOTS:
  void onReadyRead();

private:
  struct Private;
  QTimer *timer;
  QextSerialPort *port;
  QextSerialEnumerator *enumerator;
};

#endif // SERIALTHREAD_H

