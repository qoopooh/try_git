#include "eep.h"

unsigned short eepVoltMin;
unsigned short eepVoltMax;
unsigned short eepCurrMin;
unsigned short eepCurrMax;

unsigned short readEepWord(int addr)
{
  unsigned short data(0);
  char low = EEPROM.read(addr * 2);
  char high = EEPROM.read((addr * 2) + 1);

  data = word(high, low);

  return data;
}

void writeEepWord(int addr, unsigned short data)
{
  char low = lowByte(data);
  char high = highByte(data);

  EEPROM.write(addr * 2, low);
  EEPROM.write((addr * 2) + 1, high);
}
