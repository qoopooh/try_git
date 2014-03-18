#include "keyb.h"

Keyb::Keyb(TwoWire* wire)
{
  m_wire = wire;
  init();
}

void Keyb::init(void)
{
  m_wire->begin();
  setup();
}

void Keyb::setup()
{
  
//  const uint8_t k_IDAC = 0x18; // default is 0xA
  
//  const uint8_t k_finger_th_high_sens = 64;  // default is 64
//  const uint8_t k_finger_th_med_sens = 50;  // default is 64
//  const uint8_t k_finger_th_low_sens = 40;  // default is 64

  char buffer[2];

  read(buffer, CS_ENABL0, 1);
  if (buffer[0] == 0xFF)
    return;

  writeCmd(COMMAND_REG, 0x08);     // set in setup mode
  delayMicroseconds(C40_MS);

 // ---------- START define sensor keys ----------
  writeCmd(CS_ENABL0, 0x0D);       // define CapSens inputs GP03, GP02 and GP00
  delayMicroseconds(C40_MS);
  
  writeCmd(CS_ENABLE, 0x0E);       // define CapSens inputs GP11, GP12 and GP13
  delayMicroseconds(C40_MS);

//  writeCmd(CS_OTH_SET, 0x02);      //ext cint
  writeCmd(CS_OTH_SET, 0x0A);      //sens_r, ext cint
  delayMicroseconds(C40_MS);
  
  writeCmd(CS_SETL_TIME, 0xA0);    // setting time (default is A0)
  delayMicroseconds(C40_MS);
 // ---------- END define sensor keys ----------


 // ---------- START Button Backlight ----------
  // GPIO (button backlight = GPIO 1/0)
  writeCmd(GPIO_ENABLE1, 0x01);    // enable GPIO 1/0
  delayMicroseconds(C40_MS);
  
  writeCmd(DM_STRONG1, 0x01);    // define GPIO 1/0 direct output
  delayMicroseconds(C40_MS);

  // PWM ....
  writeCmd(PWM_ENABLE1, 0x01);    // define PWM for GPIO 1/0
  delayMicroseconds(C40_MS);
  
  writeCmd(PWM_MODE_DC, 0x23);    // define Duty Cycle // was 22, 24=link/15%
  delayMicroseconds(C40_MS);
  
  writeCmd(PWM_DELAY, 0x00);    // do not keep light on after button release
  delayMicroseconds(C40_MS);

  writeCmd(OP_SEL_10, 0x90);    // OR operation for GPIO 1/0
  delayMicroseconds(C40_MS);
  
  writeCmd(OPR1_PRT0_10, 0x0d);    // add Sens 0/0, 0/2, 0/3 to OR operation
  delayMicroseconds(C40_MS);
  
  writeCmd(OPR1_PRT1_10, 0x0e);    // add Sens 1/1, 1/2, 1/3 to OR operation
  delayMicroseconds(C40_MS);
 // ---------- END Button Backlight ----------
  
  
  
 // ---------- START UP key ----------
 // GPIO 1/1
  writeCmd(CS_FINGER_TH_11, 0xA0);
  delayMicroseconds(C20_MS);
  
  writeCmd(CS_IDAC_11, 0x0A); //SE1
  delayMicroseconds(C40_MS);
 
 // ---------- END UP key ----------

 
 // ---------- START DOWN key ----------
 // GPIO 0/3
  writeCmd(CS_FINGER_TH_03, 0xA0);
  delayMicroseconds(C20_MS);
  
  writeCmd(CS_IDAC_03, 0x0A); //SE1
  delayMicroseconds(C40_MS);
 
 // ---------- END DOWN key ----------
 
 // ---------- START LEFT key ----------
 // GPIO 0/0
  writeCmd(CS_FINGER_TH_00, 0xA0);
  delayMicroseconds(C20_MS);
  
  writeCmd(CS_IDAC_00, 0x0A); //SE1
  delayMicroseconds(C40_MS);
 
 // ---------- END LEFT key ----------
 
 // ---------- START RIGHT key ----------
 // GPIO 1/3
  writeCmd(CS_FINGER_TH_13, 0xA0);
  delayMicroseconds(C20_MS);
  
  writeCmd(CS_IDAC_13, 0x0A); //SE1
  delayMicroseconds(C40_MS);
 
 // ---------- END RIGHT key ----------
 
 // ---------- START OK key ----------
 // GPIO 1/2
  writeCmd(CS_FINGER_TH_12, 0xA0);
  delayMicroseconds(C20_MS);
 
  writeCmd(CS_IDAC_12, 0x0A); //SE1
  delayMicroseconds(C40_MS);
  
 // ---------- END OK key ----------
 
 // ---------- START CANCEL key ----------
 // GPIO 0/2
  writeCmd(CS_FINGER_TH_02, 0xA0);
  delayMicroseconds(C20_MS);
  
  writeCmd(CS_IDAC_02, 0x0A); //SE1
  delayMicroseconds(C40_MS);
 
 // ---------- END CANCEL key ----------
 
  
  
//  writeCmd(CS_FILTERING, 0x31);    //Enable averaging filter, 4 samples
  writeCmd(CS_FILTERING, 0xB3);    //Autoreset baseline on boot, ignore scanning on i2c, Enable averaging filter, 8 samples
  delayMicroseconds(C40_MS);
  
  writeCmd(CS_DEBOUNCE, 0x3);    //3 times 1 recognised to actually set the output to 1
  delayMicroseconds(C40_MS);
  
  
  
   // ---------- START save setup and reset ----------
  writeCmd(COMMAND_REG, 0x01);     // save the configuration to the flash
  delayMicroseconds(C300_MS);
  delayMicroseconds(C300_MS);
  
  writeCmd(COMMAND_REG, 0x06);     // do sw reset
  delayMicroseconds(C300_MS);
  delayMicroseconds(C300_MS);
  delayMicroseconds(C300_MS);
  delayMicroseconds(C300_MS);
  // ---------- END save setup and reset ----------
  
}


uint8_t Keyb::read(char* buffer, uint8_t param, uint8_t size)
{
  uint8_t cnt(0);

  write((char *)&param, 1);

  m_wire->requestFrom(CY8C_ADDRESS, size);
  if (!m_wire->available())
    return 0;
  do {
    buffer[cnt++] = m_wire->read();
  } while ((cnt<size) && m_wire->available());

  return cnt;
}

uint8_t Keyb::read_button(char* buffer, uint8_t sens)
{
  if (!sens)
    return 0;
  writeCmd(CS_READ_BUTTON, sens);

  return read(buffer, CS_READ_RAWM);
}

uint8_t Keyb::read_button_baseline(char* buffer, uint8_t sens)
{
  if (!sens)
    return 0;
  writeCmd(CS_READ_BUTTON, sens);

  return read(buffer, CS_READ_BLM);  
}

void Keyb::writeCmd(uint8_t cmd, uint8_t param)
{
  char buffer[] = { (char) cmd, (char) param };

  write(buffer);
}

void Keyb::write(char* buffer, uint8_t size)
{
  uint8_t cnt(0);
  
  m_wire->beginTransmission(CY8C_ADDRESS);
  do {
    m_wire->write(buffer[cnt++]);
  } while (cnt<size);
  m_wire->endTransmission();
}

void Keyb::workaroundSetCmdNormal()
{
  writeCmd(COMMAND_REG, 0x08);     // set in setup mode
  delayMicroseconds(C40_MS);
  writeCmd(COMMAND_REG, 0x07);     // set in normal mode
  delayMicroseconds(C40_MS);
}



