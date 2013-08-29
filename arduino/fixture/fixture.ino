/**********************************************************/
/*************** Include and Declaration ******************/
/**********************************************************/
#include <EEPROM.h>
#include "def.h"
#include "eep.h"
#include "cmdfixture.h"

const unsigned int ContactBounceDelay = 1000;
const unsigned int BlinkingLoop = 3;
const unsigned int BlinkingDelay = 250;
const unsigned int Loop500msTime = 500;

CmdFixture cmd;
int iEep;
int value;
char * input;
unsigned long contactBounceTime;
unsigned long mainTime;

/**********************************************************/
/******************* Main Structre ************************/
/**********************************************************/
void setup()
{
  int i;

  /* Configure I/O */
  pinMode(ButtonBootloader, INPUT);
  pinMode(ButtonPower, INPUT);
  pinMode(OutputReaderPower, OUTPUT);
  pinMode(StatusPowerOn, OUTPUT);
  pinMode(StatusPowerDrop, OUTPUT);
  pinMode(StatusCurrentError, OUTPUT);
  pinMode(StatusOnBoard, OUTPUT);

  /* Set serial port */
  Serial.begin(BAUD_9600);

  /* Set interrupt */
  attachInterrupt(0, setReaderPower, RISING); /* ButtonBootloader */
  
  /* Set eep value */
  eepVoltMin = readEepWord(EepAddrVoltMin);
  eepVoltMax = readEepWord(EepAddrVoltMax);
  eepCurrMin = readEepWord(EepAddrCurrMin);
  eepCurrMax = readEepWord(EepAddrCurrMax);

  /* Others */
  setPin(StatusPowerOn, LOW);           /* Power on */
  for (i=0; i<BlinkingLoop; i++) {
    delay(BlinkingDelay);
    setPin(StatusPowerDrop, LOW);       /* LED on: active LOW */
    setPin(StatusCurrentError, LOW);    /* LED on: active LOW */
    delay(BlinkingDelay);
    setPin(StatusPowerDrop, HIGH);      /* LED off: active LOW */
    setPin(StatusCurrentError, HIGH);   /* LED off: active LOW */
  }
}

void loop()
{
  int i;
  int value;

  /***** Check PC command ******/
  input = cmd.available();
  if (input != NULL) {
    cmd.handle(input);
  }
  /*****************************/

  /***** Check 500ms cycle *****/
  mainTime = millis();
  if (mainTime % Loop500msTime == 0) {
    loop500ms();
  }
  /*****************************/
}

void loop500ms()
{
  /** Check power consumption **/
  if (digitalRead(OutputReaderPower) == HIGH) {
    /*** Check while powering ****/
    char arrVal[2];
    value = analogRead(InputVoltage);
    arrVal[0] = highByte(value);
    arrVal[1] = lowByte(value);
    if (value < eepVoltMin) {
      cmd.notifyPinSet(StatusPowerDrop, LOW, arrVal);       /* LED on: active LOW */
    } else if (value > eepVoltMax) {
      cmd.notifyPinSet(StatusPowerDrop, LOW, arrVal);       /* LED on: active LOW */
    } else {
      /****** Normal voltage *******/
      setPin(StatusPowerDrop, HIGH);
      value = analogRead(InputCurrent);
      arrVal[0] = highByte(value);
      arrVal[1] = lowByte(value);
      if (value < eepCurrMin) {
        cmd.notifyPinSet(StatusOnBoard, HIGH, arrVal);
      } else if (value > eepCurrMax) {
        cmd.notifyPinSet(StatusCurrentError, LOW, arrVal);    /* LED on: active LOW */
      } else {
        /****** Normal current *******/
        setPin(StatusCurrentError, HIGH);
        setPin(StatusOnBoard, LOW);
      }
    }
  }
}

/**********************************************************/
/***************** Interrupt handler **********************/
/**********************************************************/
void setReaderPower()
{
  unsigned long time = millis();
  bool _readerPower = digitalRead(OutputReaderPower);

  if (time > contactBounceTime) {
    contactBounceTime = time + ContactBounceDelay;
    cmd.notifyPinSet(OutputReaderPower, !_readerPower, NULL);
  }
}
/**********************************************************/
/****************** Support method ************************/
/**********************************************************/
void setPin(char pin, char status)
{
  digitalWrite(pin, status);
}
/**********************************************************/
/*********************** END ******************************/
/**********************************************************/



