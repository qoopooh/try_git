#include "streader.h"

StReader::StReader(QObject *parent) :
    AaeReader(parent),
    aaeCommand(new AaeCommand(this)),
    debugSequenceCounter(0)
{
    setBaudrate(BAUD115200);
}

void StReader::onReadyRead()
{
    dataReceivedByteArray.clear();
    dataReceivedByteArray = readDataFromConnection();
    responseFlag = true;

    foreach (char ch, dataReceivedByteArray) {
        dataReceivedQueue.enqueue (ch);
    }
    while (dataReceivedQueue.length () > 0) {
        aaeCommandName = aaeCommand->checkCommand (dataReceivedQueue.dequeue ());
        emitCommand(aaeCommandName);
    }
}

void StReader::emitCommand(AaeCommand::AAE_COMMAND cmdName)
{
    raiseStatusMessage(tr("Get command no. ") + QString::number((int)cmdName));
    switch (cmdName) {
    case AaeCommand::CmdGetSerialNumber: {
        QByteArray ba = aaeCommand->getPayload ();
//        QString str = QString(ba.toHex ());
//        emit readerTypeNumber (str);
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdGetReaderType: {
        QByteArray ba = aaeCommand->getPayload ();
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdGetHardwareRevision: {
        QByteArray ba = aaeCommand->getPayload ();

        if (ba.at(0) == 0x00) {
            blMode = false;
        } else {
            blMode = true;
        }
        emit stx10BootloaderMode(blMode);
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdGetSoftwareRevision: {
        QByteArray ba = aaeCommand->getPayload ();
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdGetBootloaderRevision: {
        QByteArray ba = aaeCommand->getPayload ();
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdSwitchBlMode: {
        QByteArray ba = aaeCommand->getPayload ();
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdSwitchFwMode: {
        QByteArray ba = aaeCommand->getPayload ();
        emit dataReceived(ba.toHex ());
    }
        break;
    case AaeCommand::CmdHeartbeatInterrupt: {
        emit heartbeatSignal ();
    }
        break;
    case AaeCommand::CmdInventoryCyclicInterrupt: {
        QByteArray ba = aaeCommand->getPayload ();
        int len = (int) ba.at (1);
        if (ba.size () >= len + 2) {
//            if (readOneTagFlag) {
//                emit readOneTag(false);
//            }

            ba = ba.mid (2, len);
            emit readingEpc (ba.toHex ());
//            epc = QString(ba.data ());
//            emit readingEpcString (epc);
        }
    }
        break;
    default:
        break;
    }
}

void StReader::sendTestData()
{
  sendDataToReader(aaeCommand->getSoftwareRevisionCommand());
}
void StReader::inventorySingle()
{
  sendDataToReader(aaeCommand->inventorySingle());
}
void StReader::inventoryCyclic(bool on)
{
  if (on)
    sendDataToReader(aaeCommand->inventoryCyclic(AaeCommand::INVENTORY_ON));
  else
    sendDataToReader(aaeCommand->inventoryCyclic(AaeCommand::INVENTORY_OFF));
}

bool StReader::gotoBootloaderMode()
{
    bool completedFlag = false;

    debugSequenceCounter++;
    switch(debugSequenceCounter) {
    case 1:
        sendDataToReader(aaeCommand->getHardwareRevisionCommand());
        break;
    case 2:
        sendDataToReader(aaeCommand->getReaderTypeCommand());
        break;
    case 3:
        sendDataToReader(aaeCommand->getSoftwareRevisionCommand());
        break;
    case 4:
        //qDebug() << "gotoBootloaderMode: "<< debugSequenceCounter;
        sendDataToReader(aaeCommand->switchToBootloaderCommand());
        if (usb) disconnectReader();
        break;
    case 5:
        if (!usb) {
            if (!blMode) {
                sendDataToReader(aaeCommand->switchToBootloaderCommand());
            } else {
                disconnectReader();
                connectReader();
                debugSequenceCounter = 0;
                completedFlag = true;
            }
        }
        break;
    case 12: /* Delay for USB reader only */
        if (!blMode) {
          connectReader();
          sendDataToReader(aaeCommand->switchToBootloaderCommand());
        } else {
        }
        //qDebug() << "blMode: "<< blMode;
        debugSequenceCounter = 0;
        completedFlag = true;
        break;
    default:
        //qDebug() << "gotoBootloaderMode: "<< debugSequenceCounter;
        if (!usb) {
            disconnectReader();
            connectReader();
            debugSequenceCounter = 0;
            completedFlag = true;
        }
        break;
    }

    return completedFlag;
}
bool StReader::gotoFirmwareMode()
{
    bool completedFlag = false;

    debugSequenceCounter++;
    switch(debugSequenceCounter) {
    case 1: /* dummy command */
        sendDataToReader(aaeCommand->getHardwareRevisionCommand());
        break;
    case 2: /* switch back to firmware mode and restart */
        sendDataToReader(aaeCommand->switchToFirmwareCommand());
        if (usb) disconnectReader();
        //qDebug() << "gotoFirmwareMode: "<< debugSequenceCounter;
        break;
    case 3: /* restart waiting */
    case 4: /* restart waiting */
    case 5: /* restart waiting */
        break;
    case 6:
        if (usb) {
            connectReader();
            sendDataToReader(aaeCommand->getSoftwareRevisionCommand());
        }
        //qDebug() << "gotoFirmwareMode: "<< debugSequenceCounter;
        break;
    default:
        disconnectReader();
        debugSequenceCounter = 0;
        completedFlag = true;
        break;
    }

    return completedFlag;
}

void StReader::setUsbReader(bool _usb)
{
    usb = _usb;
}

