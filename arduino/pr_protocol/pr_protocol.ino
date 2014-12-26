/**
* Implement PR protocol
*/

#include "def.h"
#include "convert.h"
#include "command.h"

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

  readCommand();
}

char buffer[256];
int buffer_index = 0;
void readCommand() {
  int i, p_size;

  while (Serial.available() > 0) {
    int inChar = Serial.read();

    buffer[buffer_index++] = (char) inChar;

    if (inChar == '\n') {
      Command *command = fromString(buffer);
      if (command != NULL) {
        char *param = command->getParam();
        p_size = command->getParamSize();

        // Debug command information
        Serial.print("Command: ");
        Serial.println(command->getCommand());
        Serial.print("Parameter size: ");
        Serial.println(p_size);
        Serial.print("Parameter: ");
        for (i = 0; i < p_size; i++) {
          Serial.print((int)param[i]);
          Serial.print(" ");
        }
        Serial.println();
        Serial.print("String: ");
        Serial.println(command->toString(buffer, sizeof(buffer)));

        // Handle the command


        // Clean up command
        delete command;
        command = NULL;
      }

      buffer_index = 0;
    }
  }
}

Command* fromString(const char *str) {
  const int k_package_max = 128;
  int count = 0;
  const char *_str = str;
  char package[k_package_max];
  Command *command = NULL;

  while (*_str != '\0') {
    char b1 = *(_str);
    char b2 = *(_str + 1);
    if ((b1 > 'f') || (b1 < '0')
        || (b2 > 'f') || (b2 < '0')
        ) {
      break;
    }

    ++count;
    _str += 2;
  }

  if (count < 1) {
    return command;
  }

  Convert::AsciiToHex(str, count, package);
  if (count < 2) {
    command = new Command((unsigned char) package[0]);
  } else {
    command = new Command((unsigned char) package[0], &package[1], count - 1);
  }

  return command;
}

