/**
* Test reliability of cheap relay
*/

#include "def.h"

#define LED         13
#define BUTTON      2

const char *k_version = "External Interrupt V0.1";
const int k_button_delay = 250;
const int k_count_min = 1400;

volatile int state = LOW;
unsigned int count;
unsigned int count_old;

void setup()
{
  //pinMode(BUTTON, INPUT); // Set by Interrupt
  pinMode(LED, OUTPUT);
  attachInterrupt(0, blink, FALLING);  // input: D2, function,
                                      //mode: LOW/CHANGE/RISING/FALLING
  Serial.begin(115200);
  while(!Serial);

  Serial.println(k_version);
  count = 0;
}

void loop()
{
  if (count != count_old) {
    digitalWrite(LED, !digitalRead(LED));
    Serial.println(count);
    count_old = count;
  }
}

/**
  * External Interrupt D2
  */
void blink()
{
  ++ count;
}

