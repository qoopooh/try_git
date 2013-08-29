#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Arduino.h"
#include "convert.h"

class Protocol {
  public:
    static const int ReturnChar;
    static const int PacketSize = 32;
    char iVal[PacketSize/2]; /* conversion of inputPacket */
    char oVal[PacketSize/2];

    Protocol();
    ~Protocol() {};

    int available();
    char * read();
    int write(const char *, int);

  protected:
    bool completed;
    int commandValue;
    int inputPacketIdx;
    char inputPacket[PacketSize];

    void calculateResult();
};

#endif /* PROTOCOL_H */
