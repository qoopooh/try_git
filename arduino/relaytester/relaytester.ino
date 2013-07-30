/**
* Test reliability of cheap relay
*/

#include "def.h"

#define R1          13
#define R2          2
#define f_r1        flags_00.flg.bit0
#define f_r2        flags_00.flg.bit1

flags flags_00;
unsigned short count = 0;

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Relay testing");
}

void loop()
{
  ++count;
  if ((count & 0x3FFF) == 0) {
    f_r1 ^= 1;
    digitalWrite(R1, f_r1);
  }
  //delay(1000);
  //digitalWrite(R1, HIGH);
  //delay(1000);
  //digitalWrite(R1, LOW);
}

