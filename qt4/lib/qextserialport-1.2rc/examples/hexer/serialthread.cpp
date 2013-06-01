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

SerialThread::SerialThread(QString portname, QObject *parent)
    :QThread(parent)
{
  PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
  port = new QextSerialPort(portname, settings, QextSerialPort::Polling);
  connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

SerialThread::~SerialThread()
{
  //delete m_d;
  delete port;
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

