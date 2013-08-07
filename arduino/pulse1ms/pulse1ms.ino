/**
* Test reliability of cheap relay
*/

#include "def.h"

#define LED         13
#define R1          12
#define BUTTON      2

const char *k_version = "Pulse Test V1.0";
const int k_button_delay = 250;
const int k_count_min = 1400;

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(R1, OUTPUT);
  digitalWrite(R1, LOW);

  Serial.begin(115200);
  while(!Serial);

  Serial.println(k_version);
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
  delay(k_button_delay);
  digitalWrite(R1, HIGH);
  delayMicroseconds(k_count_min);
  digitalWrite(R1, LOW);
  digitalWrite(LED, LOW);
}

