#ifndef EEP_H
#define EEP_H

#include "Arduino.h"
#include <EEPROM.h>

extern unsigned short eepVoltMin;
extern unsigned short eepVoltMax;
extern unsigned short eepCurrMin;
extern unsigned short eepCurrMax;

extern unsigned short readEepWord(int addr);
extern void writeEepWord(int addr, unsigned short data);

#endif

