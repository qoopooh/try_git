#include "log.h"

Log::Log(QObject *parent) :
    QObject(parent)
{
    filename = "out.dat";
    logfile = new QFile(filename);
}

void Log::logData(const QByteArray &ba)
{
    logPacketReading(-1, ba);
}
void Log::logPacketReading (int extractState, const QByteArray &ba)
{
    if (!logfile->open(QIODevice::Append))
            return;
    date = QDate::currentDate ();
    logfile->write (date.toString ().toLatin1());
    logfile->write (" ");
    time = QTime::currentTime ();
    logfile->write (time.toString ().toLatin1());
    logfile->write (", ");
    logfile->write (getExtractStateString(extractState));
    logfile->write (", ");
    logfile->write (ba);
    logfile->write ("\r");
    logfile->close ();
}

QByteArray Log::getExtractStateString(int state)
{
    QStringList states;
    states
            << "EXTRACT_STATE_START_BYTE"
            << "EXTRACT_STATE_START_BYTE1"
            << "EXTRACT_STATE_START_BYTE2"
            << "EXTRACT_STATE_START_BYTE3"
            << "EXTRACT_STATE_START_BYTE4"
            << "EXTRACT_STATE_COMMAND"
            << "EXTRACT_STATE_COMMAND1"
            << "EXTRACT_STATE_COMMAND2"
            << "EXTRACT_STATE_LENGTH_START_BYTE"
            << "EXTRACT_STATE_LENGTH"
            << "EXTRACT_STATE_PAYLOAD_START_BYTE"
            << "EXTRACT_STATE_PAYLOAD"
            << "EXTRACT_STATE_CHECKSUM_START_BYTE"
            << "EXTRACT_STATE_CHECKSUM"
            << "EXTRACT_STATE_COMPLETE"
            ;

    if ((state < 0) || (state >= states.length ())) {
        return "EXTRACT_STATE_UNDEF";
    } else {
        return states.at (state).toLatin1();
    }
}
