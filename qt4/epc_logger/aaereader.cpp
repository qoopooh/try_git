#include "aaereader.h"

AaeReader::AaeReader(QObject *parent) :
    QThread(parent),
    serialport(new QextSerialPort(QextSerialPort::EventDriven)),
    channelName("COM1"),
    brt(BAUD38400)
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
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

    for (int i = 0; i<ports.size(); i++){
        portnames.append(ports.at(i).portName);
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
    this->serialport->setFlowControl(FLOW_OFF);
    this->serialport->setParity(PAR_NONE);
    this->serialport->setDataBits(DATA_8);
    this->serialport->setStopBits(STOP_1);
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

void AaeReader::setBaudrate(BaudRateType _brt)
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
      //qDebug() << "Written bytes:" <<
          serialport->write(dataSendByteArray);
    } else {
        qDebug() << "Error: cannot write data to serial port" << ba;
    }
}

void AaeReader::onReadyRead()
{
  qDebug() << "AaeReader::onReadyRead()";
}

void AaeReader::run()
{

}

