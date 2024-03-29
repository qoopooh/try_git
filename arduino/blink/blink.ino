/*
Blink
Turns on/off an LED, repeatedly.
*/

const int LED0 = 4;
const int LED = LED_BUILTIN; // Pin 13 has an LED on most Arduino boards
const int k_delay = 500; // unit: millisecond

void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED0, OUTPUT);     
  pinMode(LED, OUTPUT);     
}

void loop() {
  digitalWrite(LED, HIGH);    // set the LED on
  digitalWrite(LED0, LOW);    // set the LED off
  delay(k_delay);             // wait for a second
  digitalWrite(LED, LOW);     // set the LED off
  digitalWrite(LED0, HIGH);   // set the LED on
  delay(k_delay);             // wait for a second
}

