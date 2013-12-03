#include "aaecommand.h"

AaeCommand::AaeCommand(QObject *parent) :
    QObject(parent)
{
    log = new Log(this);
    protocolType = ProtocolTypeAae;
    changePrefixProtocol();
}

void AaeCommand::changePrefixProtocol()
{
    startBytesProtocol.clear();

    if(protocolType == ProtocolTypeRfe) {
        startBytesProtocol.append('R');
        startBytesProtocol.append('F');
        startBytesProtocol.append('E');
    } else {
        startBytesProtocol.append('A');
        startBytesProtocol.append('A');
        startBytesProtocol.append('E');
    }
}

AaeCommand::AAE_COMMAND AaeCommand::checkCommand(const QByteArray &ba)
{
    bool wrongPacket = false;

    extractState = EXTRACT_STATE_START_BYTE;
    for (int i=0; i<ba.length (); i++) {
        switch (extractState) {
        case EXTRACT_STATE_START_BYTE: {
            if ((ba.at (i) == startBytesProtocol.at(0))
                    && (ba.at (i + 1) == startBytesProtocol.at(1))
                    && (ba.at (i + 2) == startBytesProtocol.at(2))
                    && (ba.at (i + 3) == (char) 0x01)
                    ) {
                i += 3;
                payloadLen = 0;
                payload.clear ();
                extractState = EXTRACT_STATE_COMMAND;
            } else {
                wrongPacket = true;
            }
        }
            break;
        case EXTRACT_STATE_COMMAND: {
            cmd1 = ba.at (i);
            cmd2 = ba.at (i + 1);
            switchCheckingCmd12();
            i++;
            extractState = EXTRACT_STATE_LENGTH_START_BYTE;
        }
            break;
        case EXTRACT_STATE_LENGTH_START_BYTE: {
            if (ba.at (i) == (char)0x02) {
                extractState = EXTRACT_STATE_LENGTH;
            } else {
                wrongPacket = true;
            }
        }
            break;
        case EXTRACT_STATE_LENGTH: {
            payloadLen = (int) ba.at (i);
            if (payloadLen == 0) {
                extractState = EXTRACT_STATE_CHECKSUM_START_BYTE;
            } else {
                extractState = EXTRACT_STATE_PAYLOAD_START_BYTE;
            }
        }
            break;
        case EXTRACT_STATE_PAYLOAD_START_BYTE: {
            if (ba.at (i) == (char)0x03) {
                extractState = EXTRACT_STATE_PAYLOAD;
            } else {
                wrongPacket = true;
            }
        }
            break;
        case EXTRACT_STATE_PAYLOAD: {
            if (ba.size () > i + payloadLen) {
                payload = ba.mid (i, payloadLen);
                i += (payloadLen - 1);
                extractState = EXTRACT_STATE_CHECKSUM_START_BYTE;
            } else {
                wrongPacket = true;
            }
        }
            break;
        case EXTRACT_STATE_CHECKSUM_START_BYTE: {
            if (ba.at (i) == (char)0x04) {
                extractState = EXTRACT_STATE_CHECKSUM;
            } else {
                wrongPacket = true;
            }
        }
            break;
        case EXTRACT_STATE_CHECKSUM: {
            extractState = EXTRACT_STATE_COMPLETE;
        }
            break;
        default:
            wrongPacket = true;
            break;
        }

        if (wrongPacket) {
            log->logPacketReading((int)extractState, ba.toHex ());
            extractState = EXTRACT_STATE_START_BYTE;
            break;
        }
    }

    if (extractState != EXTRACT_STATE_COMPLETE) {
        return AaeCommand::CmdUndef;
    } else {
        extractState = EXTRACT_STATE_START_BYTE;
        return cmd;
    }
}
AaeCommand::AAE_COMMAND AaeCommand::checkCommand(const char data)
{
    bool wrongPacket = false;

    switch (extractState) {
    case EXTRACT_STATE_START_BYTE: {
        if (data == startBytesProtocol.at(0)) {
            extractState = EXTRACT_STATE_START_BYTE2;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_START_BYTE2: {
        if (data == startBytesProtocol.at(1)) {
            extractState = EXTRACT_STATE_START_BYTE3;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_START_BYTE3: {
        if (data == 'E') {
            extractState = EXTRACT_STATE_START_BYTE4;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_START_BYTE4: {
        if (data == (char) 0x01) {
            cmd1 = 0;
            cmd2 = 0;
            payloadIndex = 0;
            payloadLen = 0;
            payload.clear ();
            extractState = EXTRACT_STATE_COMMAND1;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_COMMAND1: {
        cmd1 = data;
        extractState = EXTRACT_STATE_COMMAND2;
    }
        break;
    case EXTRACT_STATE_COMMAND2: {
        cmd2 = data;
        switchCheckingCmd12();
        extractState = EXTRACT_STATE_LENGTH_START_BYTE;
    }
        break;
    case EXTRACT_STATE_LENGTH_START_BYTE: {
        if (data == (char)0x02) {
            extractState = EXTRACT_STATE_LENGTH;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_LENGTH: {
        payloadLen = (int) data;
        if (payloadLen == 0) {
            extractState = EXTRACT_STATE_CHECKSUM_START_BYTE;
        } else {
            extractState = EXTRACT_STATE_PAYLOAD_START_BYTE;
        }
    }
        break;
    case EXTRACT_STATE_PAYLOAD_START_BYTE: {
        if (data == (char)0x03) {
            extractState = EXTRACT_STATE_PAYLOAD;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_PAYLOAD: {
        payload.append (data);
        payloadIndex++;
        if (payloadIndex >= payloadLen) {
            extractState = EXTRACT_STATE_CHECKSUM_START_BYTE;
        }
    }
        break;
    case EXTRACT_STATE_CHECKSUM_START_BYTE: {
        if (data == (char)0x04) {
            extractState = EXTRACT_STATE_CHECKSUM;
        } else {
            wrongPacket = true;
        }
    }
        break;
    case EXTRACT_STATE_CHECKSUM: {
        extractState = EXTRACT_STATE_COMPLETE;
    }
        break;
    default: {
        if (data == startBytesProtocol.at(0)) {
            extractState = EXTRACT_STATE_START_BYTE2;
        } else {
            wrongPacket = true;
        }
    }
        break;
    }

    if (wrongPacket) {
        log->logPacketReading((int)extractState, &data);
        extractState = EXTRACT_STATE_START_BYTE;
    }

    if (extractState != EXTRACT_STATE_COMPLETE) {
        return AaeCommand::CmdUndef;
    } else {
        extractState = EXTRACT_STATE_START_BYTE;
        return cmd;
    }
}
void AaeCommand::switchCheckingCmd12()
{
    switch (cmd1) {
    case READER_COMMON: {
        switch (cmd2) {
        case GET_SERIAL_NUMBER: cmd = CmdGetSerialNumber; break;
        case GET_READER_TYPE: cmd = CmdGetReaderType; break;
        case GET_HARDWARE_REVISION: cmd = CmdGetHardwareRevision; break;
        case GET_SOFTWARE_REVISION: cmd = CmdGetSoftwareRevision; break;
        case GET_BOOTLOADER_REVISION: cmd = CmdGetBootloaderRevision; break;
        case SWITCH_BT_MODE: cmd = CmdSwitchBlMode; break;
        case SWITCH_FW_MODE: cmd = CmdSwitchFwMode; break;
        default: cmd = CmdUndef; break;
        }
    }
        break;
    case READER_CONTROL: {
        switch (cmd2) {
        case SET_HEART_BEAT: cmd = CmdSetHeartbeat; break;
        default: cmd = CmdUndef; break;
        }
    }
        break;
    case TAG_FUNCTIONS: {
        switch (cmd2) {
        case INVENTORY_SINGLE: cmd = CmdInventorySingle; break;
        case INVENTORY_CYCLIC: cmd = CmdInventoryCyclic; break;
        default: cmd = CmdUndef; break;
        }
    }
        break;
    case INTERRUPT: {
        switch (cmd2) {
        case HEART_BEAT_INTERRUPT: cmd = CmdHeartbeatInterrupt; break;
        case INVENTORY_CYCLIC_INTERRUPT: cmd = CmdInventoryCyclicInterrupt; break;
        default: cmd = CmdUndef; break;
        }
    }
        break;
    default: cmd = CmdUndef; break;
    }
}

QByteArray AaeCommand::setCommand(const AAE_COMMAND _command, int _payloadSize, const QByteArray &_payload)
{
    int ch = '\0';

    dataPacket.clear ();
    dataPacket.append ("AAE");
    dataPacket.append ((char) 0x01);
    switchSendingCmd12(_command, &_payloadSize);
    dataPacket.append ((char) 0x02); //EXTRACT_STATE_LENGTH_START_BYTE
    if (_payloadSize == 0) {
        dataPacket.append ('\0');
        dataPacket.append ((char) 0x04); //EXTRACT_STATE_CHECKSUM_START_BYTE
    } else {
        dataPacket.append ((char) _payloadSize);
        dataPacket.append ((char) 0x03); //EXTRACT_STATE_PAYLOAD_START_BYTE
        dataPacket.append (_payload);
        dataPacket.append ((char) 0x04); //EXTRACT_STATE_CHECKSUM_START_BYTE
    }
    for (int i=0; i <dataPacket.length (); i++) {
        ch ^= dataPacket.at (i);
    }
    dataPacket.append (ch);

    return dataPacket;
}
void AaeCommand::switchSendingCmd12(const AAE_COMMAND _command, int *_payloadSize)
{
    switch (_command) {
    case CmdGetSerialNumber: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) GET_SERIAL_NUMBER);
        *_payloadSize = 0;
    }
        break;
    case CmdGetReaderType: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) GET_READER_TYPE);
        *_payloadSize = 0;
    }
        break;
    case CmdGetHardwareRevision: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) GET_HARDWARE_REVISION);
        *_payloadSize = 0;
    }
        break;
    case CmdGetSoftwareRevision: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) GET_SOFTWARE_REVISION);
        *_payloadSize = 0;
    }
        break;
    case CmdSwitchBlMode: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) SWITCH_BT_MODE);
    }
        break;
    case CmdSwitchFwMode: {
        dataPacket.append ((char) READER_COMMON);
        dataPacket.append ((char) SWITCH_FW_MODE);
    }
        break;
    case CmdSetHeartbeat: {
        dataPacket.append ((char) READER_CONTROL);
        dataPacket.append ((char) SET_HEART_BEAT);
    }
        break;
    case CmdInventorySingle: {
        dataPacket.append ((char) TAG_FUNCTIONS);
        dataPacket.append ((char) INVENTORY_SINGLE);
    }
        break;
    case CmdInventoryCyclic: {
        dataPacket.append ((char) TAG_FUNCTIONS);
        dataPacket.append ((char) INVENTORY_CYCLIC);
    }
        break;
    default: {
        dataPacket.append ((char) 0x00);
        dataPacket.append ((char) 0x00);
        *_payloadSize = 0;
    }
        break;
    }
}

QByteArray AaeCommand::setHeartbeatCommand(const HEARTBEAT_SIGNAL hbSigal, int ms)
{
    QByteArray time;

    if (hbSigal == HEARTBEAT_ON) {
        time.append ((char) 1);
    } else {
        time.append ((char) 0);
    }
    time.append ((char) (ms / 256));
    time.append ((char) (ms % 256));

    return setCommand(AaeCommand::CmdSetHeartbeat, time.length (), time);
}
QByteArray AaeCommand::inventoryCyclic(const INVENTORY_MODE inventoryMode)
{
    QByteArray _payload;

    _payload.append ((char) inventoryMode);
    return setCommand(AaeCommand::CmdInventoryCyclic, 1, _payload);
}
QByteArray AaeCommand::inventorySingle()
{
    QByteArray _payload;

    return setCommand(AaeCommand::CmdInventorySingle, 0, NULL);
}
QByteArray AaeCommand::getPayload()
{
    return payload;
}

/* Boot Loader */
QByteArray AaeCommand::getReaderTypeCommand()
{
    return setCommand(AaeCommand::CmdGetReaderType, 0, NULL);
}
QByteArray AaeCommand::getHardwareRevisionCommand()
{
    return setCommand(AaeCommand::CmdGetHardwareRevision, 0, NULL);
}
QByteArray AaeCommand::getSoftwareRevisionCommand()
{
    return setCommand(AaeCommand::CmdGetSoftwareRevision, 0, NULL);
}
QByteArray AaeCommand::switchToBootloaderCommand()
{
    QByteArray _payload;

    _payload.append ((char) 0);
    return setCommand(AaeCommand::CmdSwitchBlMode, 1, _payload);
}
QByteArray AaeCommand::switchToFirmwareCommand()
{
    QByteArray _payload;

    _payload.append ((char) 0);
    return setCommand(AaeCommand::CmdSwitchFwMode, 1, _payload);
}
/* Boot Loader */
