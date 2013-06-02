#include "serialthread.h"
#include <QDebug>

struct SerialThread::SerialInfo
{
public:
  SerialInfo()
      : portName(QString("COM1")), baud(BAUD9600)
  { }

  QString portName;
  BaudRateType baud;
};

SerialThread::SerialThread() :
  m_info(new SerialInfo()), port(NULL)
{

  timer = new QTimer(this);
  timer->setInterval(40);
}

SerialThread::~SerialThread()
{
  delete port;
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
  if (port->bytesAvailable()) {
    QString str = QString::fromLatin1(port->readAll());
    emit data(str);
  }
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

void SerialThread::write(const QByteArray &ba)
{
  if (port != NULL) {
    port->write(ba);
    qDebug() << "w " << ba.data();
  } else {
    qDebug() << "cannot write";
  }
}

void SerialThread::run()
{
  PortSettings settings = {m_info->baud, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
  port = new QextSerialPort(settings);
  qDebug() << "port" << m_info->portName;
  port->setPortName(m_info->portName);
  if (!port->open(QIODevice::ReadWrite)) {
    emit data("Cannot open port");
    delete port;
    port = NULL;
    return;
  }
  connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  exec();
  disconnect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  port->close();
  delete port;
  port = NULL;
}

