#include "aaereader.h"

AaeReader::AaeReader(QObject *parent) :
    QThread(parent),
    serialport(new QSerialPort(this)),
    channelName("COM1"),
    brt(QSerialPort::Baud38400)
{
  connect(this->serialport, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}
AaeReader::~AaeReader()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}

QList<QString> AaeReader::discovery()
{
    QList<QString> portnames;
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (int i = 0; i<ports.size(); i++){
        portnames.append((ports.at(i).portName()));
    }

    return portnames;
}

void AaeReader::connectReader()
{
    connectReader(channelName);
}
void AaeReader::connectReader(const QString &serialportName)
{
    //qDebug() << "connectReader";
    channelName = serialportName;

    this->serialport->setPortName(channelName);
    this->serialport->setBaudRate(brt);
    this->serialport->setFlowControl(QSerialPort::NoFlowControl);
    this->serialport->setParity(QSerialPort::NoParity);
    this->serialport->setDataBits(QSerialPort::Data8);
    this->serialport->setStopBits(QSerialPort::OneStop);
    if(this->serialport->open(QIODevice::ReadWrite)) {
        emit connection(true);
        emit raiseStatusMessage(serialportName + tr(" is connecting"));
    } else {
        emit connection(false);
        emit raiseErrorMessage(tr("Cannot open ") + serialportName);
    }
}
void AaeReader::disconnectReader()
{
    //qDebug() << "disconnectReader";
    if (serialport == NULL) {
        return;
    }
    this->serialport->flush();
    this->serialport->close();
    emit connection(false);
    emit raiseStatusMessage(this->serialport->portName() + tr(" is disconnected"));
}

bool AaeReader::isConnected()
{
    if (serialport == NULL) {
        return false;
    }
    return this->serialport->isOpen();
}

void AaeReader::setBaudrate(qint32 _brt)
{
    this->brt = _brt;
}

QByteArray AaeReader::readDataFromConnection()
{
    qint16 dataSize = serialport->bytesAvailable ();
    QByteArray ba;

    ba.resize(dataSize);
    serialport->read(ba.data(), dataSize);

    return ba;
}

void AaeReader::sendDataToReader(const QByteArray &ba)
{
    dataSendByteArray.clear();
    dataSendByteArray.append(ba);
    if (serialport->isWritable()) {
        qDebug() << "Written bytes:" << serialport->write(dataSendByteArray);
    } else {
        qDebug() << "Port Closed: cannot send " << ba;
    }
}

void AaeReader::onReadyRead()
{
  qDebug() << "onReadyRead";
}

void AaeReader::run()
{

}

