#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QStringList>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = 0);

signals:

public slots:

public:
    void logData(const QByteArray &ba);
    void logPacketReading(int extractState, const QByteArray &ba);

private:
    QString filename;
    QFile *logfile;
    QDateTime dateTime;
    QDate date;
    QTime time;

    QByteArray getExtractStateString(int state);
};

#endif // LOG_H
