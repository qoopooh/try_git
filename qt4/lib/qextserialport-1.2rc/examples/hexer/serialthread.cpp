#include "serialthread.h" 

struct SerialThread::Private
{
public:
  Private()
      : darkerFactor(300), isOn(true)
  { }

  int darkerFactor;
  bool isOn;
};

SerialThread::SerialThread(QObject *parent)
    :QThread(parent)
{
  connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

SerialThread::~SerialThread()
{
  //delete m_d;
}

void SerialThread::turnOn(bool on)
{
  //m_d->isOn = on;
}

void SerialThread::turnOff(bool off)
{
  turnOn(!off);
}

QList<QString> SerialThread::discovery()
{
  QList<QString> ports;
  foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
    ports.append(info.portName);
  return ports;
}

void SerialThread::onReadyRead()
{
  QString str;

  emit data(str);
}

