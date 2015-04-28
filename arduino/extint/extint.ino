/**
* Test reliability of cheap relay
*/

#include "def.h"

#define LED         13
#define BUTTON      2

const char *k_version     = "External Interrupt V0.1";
const int k_button_delay  = 250;
const int k_buffer_size   = 20;
const long k_toggle_time  = 1000;
const long k_interrupt_toggle_time = 200;

volatile int state = LOW;
unsigned int count;
unsigned int previous_count;

char buffer[k_buffer_size];
int buffer_index = 0;

long mill         = 0;
long previous_mill = 0;
long interval     = 1000;

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
  if (count != previous_count) {
    toggleLED();
    Serial.println(count);
    previous_count = count;
    interval = k_interrupt_toggle_time;
  }

  if (Serial.available()) {
    if (buffer_index < k_buffer_size - 1) {
      buffer[buffer_index] = Serial.read();
      if (buffer[buffer_index] == '\r') {
        buffer[buffer_index + 1] = '\0';
        buffer_index = 0;
        printBuffer();
      } else {
        ++buffer_index;
      }
    } else {
      buffer[buffer_index] = '\0';
      buffer_index = 0;
      printBuffer();
    }
  }

  mill = millis();

  if (mill - previous_mill > interval) {
    previous_mill = mill;
    toggleLED();
    interval = k_toggle_time;
  }
}

void printBuffer() {
  Serial.print(buffer);
  Serial.write(' ');
  Serial.println(count);
}

/**
  * External Interrupt D2
  */
void blink()
{
  ++ count;
}

void toggleLED() {
  digitalWrite(LED, !digitalRead(LED));
}

