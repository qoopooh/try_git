#ifndef CMDFIXTURE_H
#define CMDFIXTURE_H

#include "Arduino.h"
#include "def.h"
#include "protocol.h"
#include "eep.h"

class CmdFixture {
  public:
    CmdFixture();
    ~CmdFixture() {};
    char * available();
    void handle(char * input);
    void notifyPinSet(char, char, char *);

  protected:
    Protocol p;

    void cmdRepOnly(char cmd);
    void cmdRepStatus(char cmd, char status);
    void cmdRepWord(char cmd, unsigned short value);
    void cmdRepEepVal(char cmd, int addr);
    void cmdRepAdVal(char cmd, int ch);
};

#endif /* CMDFIXTURE_H */
