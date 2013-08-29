#ifndef CONVERT_H
#define CONVERT_H

#include "Arduino.h"

class Convert {
  public:
    Convert() {};
    ~Convert() {};

    /*static int BcdToInt(int);*/
    /*static int IntToBcd(int);*/
    /*static void Hex1ToAscii(const char *, char *);*/
    static void HexToAscii(const char *, int, char *);
    static void AsciiToHex(char const *, int, char *);
    /*void AsciiToHex();*/

};

#endif /* CONVERT_H */
