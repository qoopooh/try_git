/**
* Test reliability of cheap relay
*/

#include "def.h"

#define LED         13
#define R1          12
#define BUTTON      2
#define f_r1              flags_00.flg.bit0
#define f_toggle_ena      flags_00.flg.bit1

const int k_count_min = 137;
//const int k_count_min = 138;
const int k_count_max = 145;

flags flags_00;
int count = k_count_min;

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(R1, OUTPUT);
  digitalWrite(R1, LOW);
  //digitalWrite(R1, HIGH);
  f_r1 = 0;

  Serial.begin(115200);
  while(!Serial);

  Serial.println("Pulse testing 1");
}

void loop()
{
  int in = digitalRead(BUTTON);

  if (in)
    return;

  while (!in) {
    in = digitalRead(BUTTON);
  }

  digitalWrite(LED, HIGH);
  delay(300);
  count = k_count_min;

  //for (int i=0; i<10; i++) {
    //digitalWrite(R1, HIGH);
    //delayMicroseconds(count * 2);
    //digitalWrite(R1, LOW);
    //delayMicroseconds(count);
  //}
  do {
    digitalWrite(R1, HIGH);
    delayMicroseconds(count * 2);
    digitalWrite(R1, LOW);
    delayMicroseconds(count);
  } while (++count < k_count_max);

  //if (++count > k_count_max)
    //return;
  //digitalWrite(R1, HIGH);
  //delayMicroseconds(count);
  //digitalWrite(R1, LOW);
  digitalWrite(LED, LOW);
}

