#ifndef AAECOMMAND_H
#define AAECOMMAND_H

#include <QObject>
#include <QQueue>
#include "log.h"


class AaeCommand : public QObject
{
    Q_OBJECT
public:
    typedef enum PROTOCOL_TYPE {
        ProtocolTypeAae,
        ProtocolTypeRfe,
        ProtocolTypeUndef
    }PROTOCOL_TYPE;
    typedef enum AAE_COMMAND {
        CmdGetSerialNumber,
        CmdGetReaderType,
        CmdGetHardwareRevision,
        CmdGetSoftwareRevision,
        CmdGetBootloaderRevision,
        CmdGetCurrentState,
        CmdGetStatusRegister,
        CmdSwitchBlMode,
        CmdSwitchFwMode,
        CmdGetAttenuation,
        CmdGetFrequency,
        CmdReboot,
        CmdSetHeartbeat,
        CmdSetAntennaPower,
        CmdSetAttenuation,
        CmdSetFrequency,
        CmdSetSensitivity,
        CmdRestoreFactorySettings,
        CmdSaveSettingsPermanent,
        CmdSetParam,
        CmdGetParam,
        CmdSetTagMode,
        CmdGetCurrentTagMode,
        CmdGetTagFunctionList,
        CmdInventorySingle,
        CmdInventoryCyclic,
        CmdReadFromTag,
        CmdWriteToTag,
        CmdLockTag,
        CmdKillTag,
        CmdCustomTagCommand,
        CmdHeartbeatInterrupt,
        CmdInventoryCyclicInterrupt,
        CmdStateChangedInterrupt,
        CmdStatusRegChangedInterrupt,
        CmdUndef = 0xFE
    }command;
    typedef enum AAE_RET_VALUE {
        AAE_RET_SUCCESS = 0x00,
        AAE_RET_ERR_OP_NOT_SUPPORTED = 0x50,
        AAE_RET_ERR_UNKOWN_ERR       = 0x51,
        AAE_RET_ERR_ON_EXEC_OP       = 0x52,
        AAE_RET_ERR_COULD_NOT_WRITE  = 0x53,
        AAE_RET_ERR_WRONG_PARAM_COUNT = 0x54,
        AAE_RET_ERR_WRONG_PARAM      = 0x55,
        AAE_RET_TMI_TAG_UNREACHABLE  = 0xA0,
        AAE_RET_TMI_MEM_OVERRUN      = 0xA1,
        AAE_RET_TMI_MEM_LOCKED       = 0xA2,
        AAE_RET_TMI_INSUFFICIENT_POWER = 0xA3,
        AAE_RET_TMI_WRONG_PASSWORD   = 0xA4
    }AAE_RET_VALUE;
    typedef enum AAE_STATE_VALUE {
        AAE_STATE_IDLE              = 0x00,
        AAE_STATE_SCANNING          = 0x01,
        AAE_STATE_REBOOTING         = 0x02
    }AAE_STATE_VALUE;
    typedef enum HEARTBEAT_SIGNAL {
        HEARTBEAT_OFF  = 0x00,
        HEARTBEAT_ON  = 0x01
    }HEARTBEAT_SIGNAL;
    typedef enum ANTENNA_POWER {
        ANTENNA_OFF  = 0x00,
        ANTENNA_ON  = 0x01
    }ANTENNA_POWER;
    typedef enum INVENTORY_MODE {
        INVENTORY_OFF  = 0x00,
        INVENTORY_ON     = 0x01
    }INVENTORY_MODE;
    typedef enum EXTRACT_STATE {
        EXTRACT_STATE_START_BYTE,
        EXTRACT_STATE_START_BYTE1,
        EXTRACT_STATE_START_BYTE2,
        EXTRACT_STATE_START_BYTE3,
        EXTRACT_STATE_START_BYTE4,
        EXTRACT_STATE_COMMAND,
        EXTRACT_STATE_COMMAND1,
        EXTRACT_STATE_COMMAND2,
        EXTRACT_STATE_LENGTH_START_BYTE,
        EXTRACT_STATE_LENGTH,
        EXTRACT_STATE_PAYLOAD_START_BYTE,
        EXTRACT_STATE_PAYLOAD,
        EXTRACT_STATE_CHECKSUM_START_BYTE,
        EXTRACT_STATE_CHECKSUM,
        EXTRACT_STATE_COMPLETE,
        EXTRACT_STATE_UNDEF
    }EXTRACT_STATE;

    /* From Reader SW Tool */
    enum GROUP_REPORT_ID_AAE{
        READER_COMMON  		= 0x01,
        READER_RF           	= 0x02,
        READER_CONTROL   	= 0x03,
        READER_TAG_MODE   	= 0x04,
        TAG_FUNCTIONS   	= 0x50,
        INTERRUPT           	= 0x90
    };
    enum READER_COMMON_GROUP{
        GET_SERIAL_NUMBER           = 0x01,
        GET_READER_TYPE             = 0x02,
        GET_HARDWARE_REVISION       = 0x03,
        GET_SOFTWARE_REVISION       = 0x04,
        GET_BOOTLOADER_REVISION     = 0x05,
        GET_CURRENT_SYSTEM          = 0x06,
        GET_CURRENT_STATE           = 0x07,
        GET_STATUS_REGISTER         = 0x08,
        SWITCH_BT_MODE              = 0x09,
        SWITCH_FW_MODE              = 0x0A
    };
    enum READER_TYPE{
        ST110               = 0x81,
        ST510               = 0x82,
        DONGLE100           = 0x83,
        DT110               = 0x84,
        HS110               = 0x85
    };
    enum READER_RF_GROUP{
        GET_ATTENUATION         = 0x01,
        GET_FREQUENCY           = 0x02,
        GET_SENSITIVITY         = 0x03
    };
    enum READER_CONTROL_GROUP{
        REBOOT                      = 0x01,
        SET_HEART_BEAT              = 0x02,
        SET_ANTENNA_POWER           = 0x03,
        SET_ATTENUATION             = 0x04,
        SET_FREQUENCY               = 0x05,
        SET_SENSITIVITY             = 0x06,
        RESTORE_FACTORY_SETTINGS    = 0x20,
        SAVE_SETTINGS_PERMANENT     = 0x21,
        SET_PARAM                   = 0x30,
        GET_PARAM                   = 0x31
    };
    enum READER_TAG_MODE_GROUP{
        SET_TAG_MODE                = 0x01,
        GET_CURRENT_TAG_MODE        = 0x02,
        GET_TAG_FUNCTION_LIST       = 0x03
    };
    enum TAG_FUNCTIONS_GROUP{
        INVENTORY_SINGLE            = 0x01,
        INVENTORY_CYCLIC            = 0x02,
        READ_FROM_TAG               = 0x03,
        WRITE_TO_TAG                = 0x04,
        LOCK_TAG                    = 0x05,
        KILL_TAG                    = 0x06,
        CUSTOM_TAG_COMMAND          = 0x10
    };
    enum INTERRUPT_GROUP{
        HEART_BEAT_INTERRUPT            = 0x01,
        INVENTORY_CYCLIC_INTERRUPT      = 0x02,
        STATE_CHANGED_INTERRUPT         = 0x03,
        STATUS_REG_CHANGED_INTERRUPT    = 0x04
    };
    /* From Reader SW Tool */

    explicit AaeCommand(QObject *parent = 0);

    QByteArray setCommand(const AAE_COMMAND _command, int _payloadSize, const QByteArray &_payload);
    QByteArray setHeartbeatCommand(const HEARTBEAT_SIGNAL hbSigal, int ms);
    QByteArray inventoryCyclic(const INVENTORY_MODE inventoryMode);
    QByteArray inventorySingle();
    QByteArray getPayload();
    /* Boot Loader */
    QByteArray getReaderTypeCommand();
    QByteArray getHardwareRevisionCommand();
    QByteArray getSoftwareRevisionCommand();
    QByteArray switchToBootloaderCommand();
    QByteArray switchToFirmwareCommand();
    /* Boot Loader */
signals:

public slots:
    AAE_COMMAND checkCommand(const QByteArray &ba);
    AAE_COMMAND checkCommand(const char data);

private:
    PROTOCOL_TYPE protocolType;
    QByteArray startBytesProtocol;
    AAE_COMMAND cmd;
    int payloadLen;
    QByteArray payload;

    QByteArray dataPacket;
    EXTRACT_STATE extractState;
    unsigned char cmd1;
    unsigned char cmd2;
    int payloadIndex;
    Log *log;

    void changePrefixProtocol();
    void switchCheckingCmd12();
    void switchSendingCmd12(const AAE_COMMAND _command, int *_payloadSize);
};

#endif // AAECOMMAND_H
