#ifndef CONVERT_H
#define CONVERT_H

#include "Arduino.h"

class Convert {
  public:
    Convert() {};
    ~Convert() {};

    static char * HexToAscii(const char *, int, char *);
    static char * AsciiToHex(char const *, int, char *);
};

#endif /* CONVERT_H */
