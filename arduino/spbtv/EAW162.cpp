#include "EAW162.h"

EAW162::EAW162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rs, uint8_t enable)
{
  DisplayBasicInit(db0, db1, db2, db3, 255, rs, enable, 255);
  Init();
}

EAW162::EAW162(uint8_t db0, uint8_t db1, uint8_t db2, uint8_t db3,
    uint8_t rw, uint8_t rs, uint8_t enable, uint8_t reset)
{
  DisplayBasicInit(db0, db1, db2, db3, rw, rs, enable, reset);
  Init();
}

void EAW162::sendDisplayInitCommands()
{
  const int k_init_us_delay = 100;


  delayMicroseconds(50000);
  WriteCommandByte(0x33); // set to 8 bit mode (paranoia)
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x32); // set to 4 bit mode
  delayMicroseconds(k_init_us_delay);

  WriteCommandByte(0x2A);
//  WriteCommandByte(0x2A); // 4 bit mode, 2 lines, 10 pt font, englis/russian characters
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x08); // display off
  delayMicroseconds(k_init_us_delay);
  delayMicroseconds(50000);
  WriteCommandByte(0x06); //entry mode set increment cursor by 1 not shifting display
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x13); //Character mode and internel power off
  delayMicroseconds(50000);
  
  WriteCommandByte(0x17); //Character mode and internel power on
  delayMicroseconds(50000);
  
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x01); //clear display
  delayMicroseconds(k_init_us_delay);
  delayMicroseconds(50000);
  WriteCommandByte(0x02); //return home
  delayMicroseconds(k_init_us_delay);
  WriteCommandByte(0x0C); //display on
  delayMicroseconds(k_init_us_delay);
  
  delayMicroseconds(20000);
}

/** Calculates correct ram address and sets position
 *
 * @param x new x position
 * @param line new line (LINE_1 or LINE_2)
 */
void EAW162::deviceSpecificSetPosition(unsigned char x, unsigned char line)
{
  WriteCommandByte(0x80 + line*0x40 + x);
}


void EAW162::displayOff()
{
  WriteCommandByte(0x08); // display off
  delayMicroseconds(50000);
}

void EAW162::displayOn()
{
   WriteCommandByte(0x0C); // display off
  delayMicroseconds(50000);  
}

void EAW162::displayBright(bool bright)
{
  if(bright)
    WriteCommandByte(0x17); //Character mode and internel power on
  else
    WriteCommandByte(0x13); //Character mode and internel power off

  delayMicroseconds(50000);
}


