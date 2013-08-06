/**
* Test reliability of cheap relay
*/

#include "def.h"

#define R1          13
#define BUTTON      2
#define f_r1              flags_00.flg.bit0
#define f_toggle_ena      flags_00.flg.bit1

flags flags_00;
unsigned short count = 0;

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(R1, OUTPUT);
  digitalWrite(R1, LOW);
  f_r1 = 0;

  Serial.begin(115200);
  while(!Serial);

  Serial.println("Pulse testing 1");
}

void loop()
{
  int in = digitalRead(BUTTON);
  int count = 100;

  if (in)
    return;

  while (!in) {
    in = digitalRead(BUTTON);
  }
  f_toggle_ena ^= 1;
  delay(500);

  do {
  digitalWrite(R1, HIGH);
  //delayMicroseconds(30); // works with 1 board
  delayMicroseconds(count);
  digitalWrite(R1, LOW);
  delayMicroseconds(count);
  } while (++count < 1000);
}

