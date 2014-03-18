#include "DogM162.h"

DogM162::DogM162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rs, uint8_t enable)
{
  DisplayBasicInit(db0, db1, db2, db3, 255, rs, enable, 255);
  Init();
}

DogM162::DogM162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset)
{
  DisplayBasicInit(db0, db1, db2, db3, rw, rs, enable, reset);
  Init();
}


DogM162::DogM162()
{
}

void DogM162::DisplayBasicInit(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset)
{
  _rw_pin = rw;
  _rs_pin = rs;
  _enable_pin = enable;
  _reset_pin = reset;
  _data_pins[0] = db0;
  _data_pins[1] = db1;
  _data_pins[2] = db2;
  _data_pins[3] = db3;
}


void DogM162::sendDisplayInitCommands()
{
  const int k_init_us_delay = 100;

  delayMicroseconds(50000);
  WriteCommandByte(0x33); // function set lines, interface
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x32); // function set lines, interface
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x29); // function set lines, interface
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x14); // function set bias, OSC
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x72);//78// function set contrast
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x55);// function set power, icon, contrast
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x6E); //6D// function set follower
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x0C);// funktion set display on/off
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x01); // clear display
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x06); // entry mode set
  delayMicroseconds(k_init_us_delay);
  delayMicroseconds(20000);
}

void DogM162::Init()
{
  pinMode(_rs_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);
  if (_rw_pin != 255) {
    pinMode(_rw_pin, OUTPUT);
    digitalWrite(_rw_pin, LOW);
  }
  if (_reset_pin != 255) {
    pinMode(_reset_pin, OUTPUT);
    digitalWrite(_reset_pin, HIGH);
  }
  digitalWrite(_rs_pin, HIGH);
  digitalWrite(_enable_pin, LOW);

  sendDisplayInitCommands();
}

void DogM162::WriteCommandByte(int cmd)
{
  digitalWrite(_rs_pin, LOW);
  SetData(cmd);
}

void DogM162::WriteDataByte(int cmd)
{
  digitalWrite(_rs_pin, HIGH);
  SetData(cmd);
}

void DogM162::write4bitsShift(uint8_t value)
{
  for (int i=0; i < 4; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }
}

void DogM162::SetData(int value)
{
  const int k_high_us_delay = 50;
  const int k_low_us_delay = 20;

  delayMicroseconds(k_low_us_delay);
  write4bitsShift(value>>4); // Oberes Nibble

  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(k_high_us_delay);

  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(k_low_us_delay);

  write4bitsShift(value); // Unteres Nibble
  delayMicroseconds(k_low_us_delay);

  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(k_high_us_delay);

  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(k_low_us_delay);
}

/** Clear Display
 *
 * Write spaces into DDRAM and set address to (0,0)
 *
 */
void DogM162::Clear()
{
  WriteCommandByte(0x01);
  delayMicroseconds(50000);
}

/** Set Position to (x,line)
 *
 * @param x new x position
 * @param line new line (LINE_1 or LINE_2)
 */
void DogM162::SetPosition(unsigned char x, unsigned char line)
{
  deviceSpecificSetPosition(x,line);
}

/** Calculates correct ram address and sets position
 *
 * @param x new x position
 * @param line new line (LINE_1 or LINE_2)
 */
void DogM162::deviceSpecificSetPosition(unsigned char x, unsigned char line)
{
  WriteCommandByte(0x80 + line*40 + x);
}

/** Write a character at the current position
 *
 * @param character character
 */
void DogM162::WriteCharacter(char character)
{
  WriteDataByte(character);
}

/** Write a character at position (x,line)
 *
 * @param character character
 * @param x x-position (starting with 0)
 * @param line (LINE_1, LINE_2 or LINE3)
 */
void DogM162::WriteCharacter(char character,unsigned char x,unsigned char line)
{
  SetPosition(x,line);
  WriteCharacter(character);
}

/** Write a null terminated string at the current position
 *
 * @param string string
 */
void DogM162::WriteString(char* str)
{
  digitalWrite(_rs_pin, HIGH);

  while (str[0]  != 0x00)
  {
    delayMicroseconds(30);
    SetData(str[0]);
    ++str;
  }
}

/** Write a null terminated string at position (x,line)
 *
 * @param string string
 * @param x x-position (starting with 0)
 * @param line (LINE_1, LINE_2 or LINE3)
 */
void DogM162::WriteString(char* str, unsigned char x, unsigned char line)
{
    SetPosition(x,line);
    WriteString(str);
}

inline size_t DogM162::write(uint8_t value)
{
  WriteDataByte(value);
  return 1; // assume sucess
}

