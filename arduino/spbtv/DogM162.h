#ifndef __DOGM162_H
#define __DOGM162_H

#include "Arduino.h"

class DogM162 : public Print {
public:
  static const unsigned char LINE_1 = 0;
  static const unsigned char LINE_2 = 1;

  DogM162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
      uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset);
  DogM162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
      uint8_t rs, uint8_t enable);
  void Clear();
  void SetPosition(unsigned char x, unsigned char line);
  void WriteCharacter(char character);
  void WriteCharacter(char character,unsigned char x,unsigned char line);
  void WriteString(char* str);
  void WriteString(char* str, unsigned char x, unsigned char line);

  virtual size_t write(uint8_t);
  using Print::write;

protected: 
  DogM162();
  void Init();  
  void DisplayBasicInit(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
			uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset);

  void WriteCommandByte(int cmd);

private:

  virtual void sendDisplayInitCommands();
  virtual void deviceSpecificSetPosition(unsigned char x, unsigned char line);

  void write4bitsShift(uint8_t value);
  void WriteDataByte(int cmd);
  void SetData(int data);

  uint8_t _rs_pin; // LOW: command.  HIGH: character.
  uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
  uint8_t _enable_pin; // activated by a HIGH pulse.
  uint8_t _reset_pin;
  uint8_t _data_pins[4];
};

#endif /* __DOGM162_H */

