#include "serialthread.h"

struct SerialThread::SerialInfo
{
public:
  SerialInfo()
      : portName("COM1"), baud(BAUD9600)
  { }

  QString portName;
  BaudRateType baud;
};

SerialThread::SerialThread()
{
  //PortSettings settings = {m_info->baud, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
  //port = new QextSerialPort(m_info->portName, settings, QextSerialPort::Polling);

  //timer = new QTimer(this);
  //timer->setInterval(40);
  //connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

SerialThread::~SerialThread()
{
  //delete port;
}

void SerialThread::turnOn(bool /*on*/)
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

void SerialThread::setPort(const QString &name)
{
  m_info->portName = name;
}

void SerialThread::setBaud(const QString &baud)
{
  if (baud == "9600") {
    m_info->baud = BAUD9600;
  } else if (baud == "38400") {
    m_info->baud = BAUD38400;
  } else {
    m_info->baud = BAUD115200;
  }
}

void run()
{
  //exec();
}

