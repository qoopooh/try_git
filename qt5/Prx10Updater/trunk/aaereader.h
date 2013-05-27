#ifndef AAEREADER_H
#define AAEREADER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QQueue>
/*#include <qextserialport.h>*/
/*#include <qextserialenumerator.h>*/
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>

class AaeReader : public QObject
{
    Q_OBJECT
public:
    explicit AaeReader();

    QList<QString> discovery();
    void connectReader();
    void connectReader(const QString &serialportName);
    /* Boot Loader */
    /* Boot Loader */

signals:
    void connection(bool connected);
    void raiseErrorMessage(const QString &_error);
    void raiseStatusMessage(const QString &_msg);
    void dataReceived(const QByteArray &ba);
    void dataReceivedString(const QString &_msg);

    void readingEpc(const QByteArray &);
    void readingEpcString(QString);

public slots:
    void disconnectReader();

protected slots:

protected:
    typedef unsigned char BYTE;
    typedef unsigned int UINT;
    typedef unsigned short WORD;
    typedef unsigned long DWORD;

    bool responseFlag;
    QByteArray dataReceivedByteArray;
    QByteArray dataSendByteArray;
    QQueue<char> dataReceivedQueue;

    /* Boot Loader */
    /* Boot Loader */

    void setBaudrate(qint32 _brt);
    QByteArray readDataFromConnection();
    void sendDataToReader(const QByteArray &ba);

private:
    QSerialPort *serialport;
    QString channelName;
    qint32 brt;
};

#endif // AAEREADER_H
