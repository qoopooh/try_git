#ifndef COMMAND_H
#define COMMAND_H

#define CMD_SOFTWARE_VERSION    0x00
#define CMD_HARDWARE_VERSION    0x01
#define CMD_READER_ID           0x02
#define CMD_READER_MODEL        0x03
#define CMD_READER_STATUS       0x04
#define CMD_RD_WR_INDEX         0x05
#define CMD_BATTERY_STATUS      0x06
#define CMD_READER_PROTOCOL     0x0A
#define CMD_READER_TYPE         0x0B
#define CMD_READER_MODULE_VER   0x0C
#define CMD_SET_TAG_TYPE        0x10
#define CMD_SET_MEM_BANK        0x11
#define CMD_SET_START_ADDRESS   0x12
#define CMD_SET_DATA_LENGTH     0x13
#define CMD_SET_FREQUENCY       0x14
#define CMD_SET_DATETIME        0x15
#define CMD_SET_BLUETOOTH_KEY   0x16
#define CMD_SET_AUTO_SENT       0x17
#define CMD_SET_BUZZER          0x18
#define CMD_SET_SENT_DIRECT     0x19
#define CMD_SET_ATTENUATION     0x1A
#define CMD_GET_TAG_TYPE        0x20
#define CMD_GET_MEM_BANK        0x21
#define CMD_GET_START_ADDRESS   0x22
#define CMD_GET_DATA_LENGTH     0x23
#define CMD_GET_FREQUENCY       0x24
#define CMD_GET_DATETIME        0x25
#define CMD_GET_BLUETOOTH_KEY   0x26
#define CMD_GET_AUTO_SENT       0x27
#define CMD_GET_BUZZER          0x28
#define CMD_GET_SENT_DIRECT     0x29
#define CMD_GET_ATTENUATION     0x2A
#define CMD_STOP_AUTO_READ      0x30
#define CMD_START_AUTO_READ     0x31
#define CMD_READ_ONCE           0x32
#define CMD_WRITE_TAG           0x40
#define CMD_WRITE_TAG_ACCESS    0x41
#define CMD_LOCK_TAG            0x50
#define CMD_KILL_TAG            0x51
#define CMD_LOCK_WITH_PASSWD    0x52
#define CMD_DOWNLOAD_NEXT       0x60
#define CMD_DOWNLOAD_RECORD     0x61
#define CMD_ERASE_MEM           0x62
#define CMD_NOTIFY_SOUND        0x74
#define CMD_NOTIFY_DISPLAY      0x75
#define CMD_IO_BUZZER_OFF       0xF0
#define CMD_IO_BUZZER_ON        0xF1
#define CMD_RESET_READER        0xF2

#include "Arduino.h"
#include "convert.h"

class Command {
  public:
    Command(unsigned char id);
    Command(unsigned char id, char *param, int p_size);
    Command(unsigned char id, char param);
    /*Command(unsigned char id, char param1, char param2); // remove since confusion*/
    ~Command();

    unsigned char getId();
    int getParamSize();
    char* getParam();
    int toString(char* buffer, int buffer_size);
    void setParamString(bool);  // set true if param is string, it does not need to convert on toString()
    void setDispatched();
    bool isDispatched();

  private:
    unsigned char m_id;
    char* m_param;
    int m_param_size;
    bool m_param_string;
    bool m_dispatch;
};

#endif /* COMMAND_H */
