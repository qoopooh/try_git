#ifndef COMMAND_H
#define COMMAND_H

#include "Arduino.h"

class Command {
  public:
    Command(unsigned char);
    Command(unsigned char cmd, char *param, int p_size);
    ~Command();

    unsigned char getCommand();
    int getParamSize();
    char * getParam();

  private:
    unsigned char m_cmd;
    char * m_param;
    int m_param_size;
};

#endif /* COMMAND_H */
