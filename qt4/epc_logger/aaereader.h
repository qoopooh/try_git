#ifndef AAEREADER_H
#define AAEREADER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <qextserialport.h>
#include <qextserialenumerator.h>

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
    void readingEpcString(const QString &);
    void readingTagCode(const QString &);
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
    void attenuation(const int &attn);

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

    void setBaudrate(BaudRateType _brt);
    QByteArray readDataFromConnection();
    void sendDataToReader(const QByteArray &ba);

private:
    QextSerialPort *serialport;
    QString channelName;
    BaudRateType brt;
    int waitTimeout;
    QMutex mutex;
    bool quit;
};

#endif // AAEREADER_H
