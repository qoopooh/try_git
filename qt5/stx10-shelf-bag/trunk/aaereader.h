#ifndef AAEREADER_H
#define AAEREADER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>

class AaeReader : public QThread
{
    Q_OBJECT
public:
    AaeReader(QObject *parent = 0);
    virtual ~AaeReader();

    static QList<QString> discovery();
    void connectReader();
    void connectReader(const QString &serialportName);
    bool isConnected();

    void startSlave(const QString &portName, int waitTimeout, const QString &response);
    void run();

signals:
    void connection(bool connected);
    void raiseErrorMessage(const QString &_error);
    void raiseStatusMessage(const QString &_msg);
    void dataReceived(const QByteArray &ba);
    void dataReceivedString(const QString &_msg);

    void readingEpc(const QByteArray &);
    void readingEpcString(QString);
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:
    void disconnectReader();

protected slots:
    virtual void onReadyRead();

protected:
    typedef unsigned char BYTE;
    typedef unsigned int UINT;
    typedef unsigned short WORD;
    typedef unsigned long DWORD;

    bool responseFlag;
    QByteArray dataReceivedByteArray;
    QByteArray dataSendByteArray;
    QQueue<char> dataReceivedQueue;

    void setBaudrate(qint32 _brt);
    QByteArray readDataFromConnection();
    void sendDataToReader(const QByteArray &ba);

private:
    QSerialPort *serialport;
    QString channelName;
    qint32 brt;
    int waitTimeout;
    QMutex mutex;
    bool quit;
};

#endif // AAEREADER_H
