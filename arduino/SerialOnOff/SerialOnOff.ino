/*
  Serial Event example

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

// constants
const int OUT = 9;
const int LED = 13;

// global variables
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(100);

  pinMode(OUT, OUTPUT);     
  pinMode(LED, OUTPUT);     
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    handleString(inputString);
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void handleString(String msg) {
  msg.toLowerCase();

  if (msg.indexOf("on") >= 0) {
    digitalWrite(OUT, HIGH);
    digitalWrite(LED, HIGH);    // set the LED on
  } else if (msg.indexOf("off") >= 0) {
    digitalWrite(OUT, LOW);
    digitalWrite(LED, LOW);    // set the LED on
  }
}

