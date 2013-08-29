#include "cmdfixture.h"

CmdFixture::CmdFixture()
{
}

/**********************************************************/
/****************** Command handler ***********************/
/**********************************************************/
char * CmdFixture::available()
{
  if (p.available() > 0) {
    return p.read();
  }

  return NULL;
}

void CmdFixture::handle(char * input)
{
  int cntIdx = 0;
  char cmd = input[1];
  char len = input[0];

  if (len < 1) return;
  switch (cmd) {
    /********** Analog and EEPROM value *******************/
    case 0x00: /* get eep voltage min */
      cmdRepWord(cmd, eepVoltMin);
      break;
    case 0x01: /* get eep voltage max */
      cmdRepWord(cmd, eepVoltMax);
      break;
    case 0x02: /* get eep current min */
      cmdRepWord(cmd, eepCurrMin);
      break;
    case 0x03: /* get eep current max */
      cmdRepWord(cmd, eepCurrMax);
      break;
    case 0x04: /* get AD voltage value */
      cmdRepAdVal(cmd, 0);
      break;
    case 0x05: /* get AD current value */
      cmdRepAdVal(cmd, 1);
      break;
    /******************************************************/

    /************ Digital monitoring **********************/
    case 0x10: /* get Digital 13 (PB5) status */
      cmdRepStatus(cmd, digitalRead(StatusOnBoard));
      break;
    case 0x11: /* get Digital 5 (PD5) status */
      cmdRepStatus(cmd, digitalRead(StatusPowerDrop));
      break;
    case 0x12: /* get Digital 7 (PD7) status */
      cmdRepStatus(cmd, digitalRead(StatusCurrentError));
      break;
    case 0x13: /* get Digital x (Pxx) status */
      cmdRepStatus(cmd, digitalRead(StatusPowerOn));
      break;
    case 0x14: /* get Digital 4 (PD4) status */
      cmdRepStatus(cmd, digitalRead(OutputReaderPower));
      break;
    case 0x15: /* get Bootloader switch status */
      cmdRepStatus(cmd, digitalRead(ButtonBootloader));
      break;
    case 0x16: /* get power switch status */
      cmdRepStatus(cmd, digitalRead(ButtonPower));
      break;
    /******************************************************/

    /***************** EEPROM setting *********************/
    case 0x20: /* set eep voltage min */
      if (len == 3) {
        eepVoltMin = word(input[2], input[3]);
        writeEepWord(EepAddrVoltMin, eepVoltMin);
        cmdRepOnly(cmd);
      }
      break;
    case 0x21: /* set eep voltage max */
      if (len == 3) {
        eepVoltMax = word(input[2], input[3]);
        writeEepWord(EepAddrVoltMax, eepVoltMax);
        cmdRepOnly(cmd);
      }
      break;
    case 0x22: /* set eep current min */
      if (len == 3) {
        eepCurrMin = word(input[2], input[3]);
        writeEepWord(EepAddrCurrMin, eepCurrMin);
        cmdRepOnly(cmd);
      }
      break;
    case 0x23: /* set eep current max */
      if (len == 3) {
        eepCurrMax = word(input[2], input[3]);
        writeEepWord(EepAddrCurrMax, eepCurrMax);
        cmdRepOnly(cmd);
      }
      break;
    /******************************************************/

    /***************** Digital setting ********************/
    case 0x30: /* set Digital 13 (PB5) */
      if (len == 2) {
        digitalWrite(StatusOnBoard, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x31: /* set Digital 5 (PD5) */
      if (len == 2) {
        digitalWrite(StatusPowerDrop, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x32: /* set Digital 7 (PD7) */
      if (len == 2) {
        digitalWrite(StatusCurrentError, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x33: /* set Digital x (Pxx) */
      if (len == 2) {
        digitalWrite(StatusPowerOn, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x34: /* set Digital 4 (PD4) */
      if (len == 2) {
        digitalWrite(OutputReaderPower, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x35: /* set Digital 2 (PD2): input button */
      if (len == 2) {
        digitalWrite(ButtonBootloader, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    case 0x36: /* set Digital 3 (PD3): input button */
      if (len == 2) {
        digitalWrite(ButtonPower, input[2]);
        cmdRepOnly(cmd);
      }
      break;
    /******************************************************/
    case 0x80: /* reserved for notify() */
      break;
    case 0x88: /* firmware version */
      Serial.print("88");
      Serial.println(FirmwareVersion);
      break;

    /***************** Error command **********************/
    case 0xFE: /* MCU reset */
      if (len == 4) {
        if ((input[2] == 'R') || (input[3] == 'S') || (input[4] == 'T')) {
          //while(true); /* waiting for watchdog reset */
        }
      }
      /* no break: it's just fake command */
    default:
        cmdRepOnly(0xFF);
      break;
  }
}

void CmdFixture::notifyPinSet(char pin, char status, char *value)
{
  digitalWrite(pin, status);
  p.oVal[0] = 0x80;
  p.oVal[1] = pin;
  p.oVal[2] = status;
  if (pin != OutputReaderPower) {
    p.oVal[3] = *value;
    p.oVal[4] = *(value + 1);
    p.write(p.oVal, 5);
  } else {
    p.write(p.oVal, 3);
  }
}

/**********************************************************/
/****************** Support method ************************/
/**********************************************************/
void CmdFixture::cmdRepOnly(char cmd)
{
  p.oVal[0] = cmd;
  p.write(p.oVal, 1);
}

void CmdFixture::cmdRepStatus(char cmd, char status)
{
  p.oVal[0] = cmd;
  p.oVal[1] = status;
  p.write(p.oVal, 2);
}

void CmdFixture::cmdRepWord(char cmd, unsigned short value)
{
  p.oVal[0] = cmd;
  p.oVal[1] = highByte(value);
  p.oVal[2] = lowByte(value);

  p.write(p.oVal, 3);
}

void CmdFixture::cmdRepEepVal(char cmd, int addr)
{
  unsigned short value = readEepWord(addr);

  cmdRepWord(cmd, value);
}

void CmdFixture::cmdRepAdVal(char cmd, int ch)
{
  unsigned short value = analogRead(ch);

  cmdRepWord(cmd, value);
}

