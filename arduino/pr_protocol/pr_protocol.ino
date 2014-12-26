/**
* Implement PR protocol
*/

#include "def.h"
#include "convert.h"

/* members */
char output[80];
unsigned short count;


/* methods */

void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(BAUD_9600);
  count = 0;
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(DELAY);
  digitalWrite(LED, LOW);
  delay(DELAY);

  //int sensorValue = analogRead(A0);
  //Serial.println(++count);
  Serial.println(Convert::HexToAscii((const char *) &(++count), 2, output));
}

