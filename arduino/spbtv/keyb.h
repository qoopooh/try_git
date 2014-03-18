#ifndef __KEYB_H
#define __KEYB_H

#include <Arduino.h>
#include <Wire.h>
#include "cy8c20110.h"

class Keyb {
public:
  static const uint8_t CY8C_ADDRESS = 0;
  static const uint16_t C20_MS = 20000;
  static const uint16_t C40_MS = 40000;
  static const uint16_t C300_MS = 56000;
  static const uint8_t k_i2c_buff_sz = 2;

  Keyb(TwoWire* wire);
  void init();
  void setup();
  uint8_t read(char* buffer, uint8_t param=CS_READ_STATUSM, uint8_t size=k_i2c_buff_sz);
  uint8_t read_button(char* buffer, uint8_t sens);
  uint8_t read_button_baseline(char* buffer, uint8_t sens);
  void workaroundSetCmdNormal();

protected:
  void writeCmd(uint8_t cmd, uint8_t param);
  void write(char* buffer, uint8_t size=k_i2c_buff_sz);

private:
  TwoWire* m_wire;
};

#endif /* __KEYB_H */

