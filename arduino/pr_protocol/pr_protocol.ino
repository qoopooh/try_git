/**
* Implement PR protocol
*/

#include "def.h"
#include "convert.h"
#include "command.h"
#include "reader.h"

/* members */

char out_buff[80];
char in_buff[256];
unsigned short in_buff_index = 0;
int d12_state = LOW;
int last_d12_state = LOW;
long last_d12_debounce = 0;
bool tx_pending = false;
Command* rx_command;
Command* tx_command;
Reader reader;

/* methods */

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(D12, INPUT);

  Serial.begin(BAUD_115200);
}

void loop() {
  if (reader.isAutoRead()) {
    digitalWrite(LED, HIGH);
    delay(DELAY);
    digitalWrite(LED, LOW);
    delay(DELAY);
  }

  readPin();
  readSerial();
  writeSerial();
  if (reader.isCommandAvailable())
    tx_command = reader.getCommand();
}

/**
* Simulate data as EPC (24 chars)
*/
void readPin() {
  int reading = digitalRead(D12);

  if (reading != last_d12_state)
    last_d12_debounce = millis();

  //if ((millis() - last_d12_debounce) > DEBOUNCE_DELAY) {
    if (reading != d12_state) {
      d12_state = reading;

      //if (d12_state == HIGH) {
      if (reader.isAutoRead()) {
        // Send EPC
        String epc = String(last_d12_debounce, DEC);
        epc.toCharArray(out_buff, sizeof(out_buff));
        tx_pending = true;
      }
    }
    //}

  last_d12_state = d12_state;
}

/**
* Read data from serial port
*/
void readSerial() {
  int i, p_size;

  while (Serial.available() > 0) {
    int inChar = Serial.read();

    in_buff[in_buff_index++] = (char) inChar;

    if (inChar == '\r') {
      if (rx_command != NULL) {
        // Clean up command
        delete rx_command;
        rx_command = NULL;
      }

      rx_command = fromString(in_buff);
      if (rx_command != NULL) {
        char *param = rx_command->getParam();
        p_size = rx_command->getParamSize();

        // Debug command information
        //Serial.print("Command: ");
        //Serial.println(rx_command->getId());
        //Serial.print("Parameter size: ");
        //Serial.println(p_size);
        //Serial.print("Parameter: ");
        //for (i = 0; i < p_size; i++) {
          //Serial.print((int)param[i]);
          //Serial.print(" ");
        //}
        //Serial.println();
        //Serial.print("String: ");
        //rx_command->toString(in_buff, sizeof(in_buff));
        //Serial.println(in_buff);

        // Handle the command
        reader.handle(rx_command);

      }

      in_buff_index = 0;
    }
  }
}

/**
* Convert string to PR command
*/
Command* fromString(const char *str) {
  const int k_package_max = 128;
  char package[k_package_max];
  const char *t_str = str;
  unsigned short t_cnt = 0;
  Command *command = NULL;

  while (*t_str != '\0') {
    char b1 = *(t_str);
    char b2 = *(t_str + 1);
    if ((b1 > 'f') || (b1 < '0')
        || (b2 > 'f') || (b2 < '0')
        ) {
      break;
    }

    ++t_cnt;
    t_str += 2;
  }

  if (t_cnt < 1) {
    return command;
  }

  Convert::AsciiToHex(str, t_cnt, package);
  if (t_cnt < 2) {
    command = new Command((unsigned char) package[0]);
  } else {
    command = new Command((unsigned char) package[0], &package[1], t_cnt - 1);
  }

  return command;
}

/**
* Write command to serial port
*/
void writeSerial() {
  if (tx_pending) {
    Serial.println(out_buff);
    tx_pending = false;
    return; // excute tx command on next loop
  }
  if ((tx_command != NULL) && (!tx_command->isDispatched())) {
    int last = tx_command->toString(out_buff, sizeof(out_buff));
    //out_buff[last] = NEW_LINE;
    Serial.println(out_buff);
    tx_command->setDispatched();
  }
}

