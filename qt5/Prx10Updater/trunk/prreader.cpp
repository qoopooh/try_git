#include "prreader.h"

PrReader::PrReader(QObject *parent) :
    AaeReader()
{
    setBaudrate(38400);
}

void PrReader::onReadyRead()
{
    bool carriageReturnFlag = false;

    dataReceivedByteArray.clear();
    dataReceivedByteArray = readDataFromConnection();
    responseFlag = true;

    foreach (char ch, dataReceivedByteArray) {
        if (ch == '\r') {
            carriageReturnFlag = true;
        }
        dataReceivedQueue.enqueue (ch);
    }
    if (carriageReturnFlag) {
        processCommand();
    }
}

void PrReader::processCommand()
{
    QByteArray cmdPacket;
    QChar dataTmp;

    cmdPacket.clear();
    while (dataReceivedQueue.length () > 0) {
        dataTmp = dataReceivedQueue.dequeue();
        if ((dataTmp == '\r') && (cmdPacket.size() > 1)) {
            getCommand(cmdPacket);
        } else {
            cmdPacket.append(dataTmp);
        }
//            aaeCommandName = aaeCommand->checkCommand (dataReceivedQueue.dequeue());
//            emitCommand(aaeCommandName);
    }
}
PrReader::PR_COMMAND PrReader::getCommand(const QByteArray &_ba)
{
    PrReader::PR_COMMAND _prCommand;
    QByteArray _cmdOnly;
    int _cmdValue;

    _cmdOnly = QByteArray::fromHex(_ba.mid(0, 2));
    _cmdValue = (int) _cmdOnly.at(0);
    switch(_cmdValue) {
    case 0x00:
        _prCommand = CmdGetFirmwareVersion00;
        break;
    default:
        _prCommand = CmdUndef;
        break;
    }

    return _prCommand;
}

void PrReader::emitCommand(PrReader::PR_COMMAND _prCommand)
{
    raiseStatusMessage(tr("Get command no. ") + QString::number((int)_prCommand));
    switch (_prCommand) {
    case PrReader::CmdGetFirmwareVersion00: {
//        QByteArray ba = aaeCommand->getPayload ();
//        emit dataReceived(ba.toHex ());
    }
        break;
    default:
        break;
    }
}


