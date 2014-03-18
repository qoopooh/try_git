// spbtv.ino
// ------------
// Creation Date: 07.12.2011
// Copyright (C) SPB TV AG 2007-2012 - All Rights Reserved
// Author: Alexander Dovgaluk <alexander.dovgaluk@spbtv.com>

// pinout:
//   7, 4, 3, 2, 12, 8 - lcd

#include <Wire.h>
#include <avr/pgmspace.h>
#include "EAW162.h"
#include "keyb.h"

#define DEV_MODE 0

// Josef: moved up because needed in setupBandDimmer()
int led7brightnessOn = 100;
int led7brightnessOff = 0;

const char *FW_VERSION = "0.0.5b";

enum {
  PIN_OUTP102_5 = 5,
  PIN_OUTP102_4 = 6,
  PIN_OUTP102_3 = 9,
  PIN_LED = 13,
  PIN_SERVER_POWER = 11,
  PIN_USB_POWER = A6,
  
/* light band dimming */ // Josef
// WARNING: these pins MUST match the bit order of values_dim_mux_hi and values_dim_mux_lo
  DIM_BAND_1   = A3,
  DIM_BAND_2_3 = A2,
  DIM_BAND_4_5 = A1,
  DIM_BAND_6_7 = A0,
  DIM_MUX      = 12

};


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// START: PWM Light band // Josef
// ---------------------------------------------------------------------------------------
#define NUM_PWN_CHANNELS 7

uint8_t pwm_multiplex_idx = 0;
uint8_t pwm_counter = 0;
uint8_t pwm_target_value[NUM_PWN_CHANNELS];
uint8_t saved_pwm_target_value[NUM_PWN_CHANNELS];

uint8_t values_dim_mux_hi, values_dim_mux_lo;

#define PORT_DIM_MASK_CLEAR  0b11110000
#define PORT_DIM_MASK_SET_LO 0b00001111
#define PORT_DIM_MASK_SET_HI 0b00000111

#define DIM_MUX_HIGH_MASK 0b00010000
#define DIM_MUX_LOW_MASK  0b11101111

#define DIM_BAND_1_CLR_MX (values_dim_mux_lo &= 0b11110111)
#define DIM_BAND_2_CLR_MX (values_dim_mux_hi &= 0b11111011)
#define DIM_BAND_3_CLR_MX (values_dim_mux_lo &= 0b11111011)
#define DIM_BAND_4_CLR_MX (values_dim_mux_hi &= 0b11111101)
#define DIM_BAND_5_CLR_MX (values_dim_mux_lo &= 0b11111101)
#define DIM_BAND_6_CLR_MX (values_dim_mux_hi &= 0b11111110)
#define DIM_BAND_7_CLR_MX (values_dim_mux_lo &= 0b11111110)


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// version for MULTIPLEXED PWM
ISR(TIMER2_OVF_vect)
{
  // detunes band PWM and keyboard PWM so keyboard does not flicker
  TCNT2  = 0x01;
  
  if(0 == pwm_multiplex_idx)
  {
    PORTC = (PORTC & PORT_DIM_MASK_CLEAR) | values_dim_mux_lo;
  }
  else
  {
    PORTC = (PORTC & PORT_DIM_MASK_CLEAR) | values_dim_mux_hi;
  }

  if(0 == pwm_counter)
  {
    memcpy(saved_pwm_target_value, pwm_target_value, NUM_PWN_CHANNELS);
      
    if(0 == pwm_multiplex_idx)
    {
      PORTB |= DIM_MUX_HIGH_MASK;

      values_dim_mux_hi = PORT_DIM_MASK_SET_HI;
      pwm_multiplex_idx = 1;
    }
    else
    {
      PORTB &= DIM_MUX_LOW_MASK;
            
      values_dim_mux_lo = PORT_DIM_MASK_SET_LO;
      pwm_multiplex_idx = 0;
    }
  }

  if(0 == pwm_multiplex_idx)
  {
    if(pwm_counter == saved_pwm_target_value[0])
      DIM_BAND_1_CLR_MX;
    if(pwm_counter == saved_pwm_target_value[2])
      DIM_BAND_3_CLR_MX;
    if(pwm_counter == saved_pwm_target_value[4])
      DIM_BAND_5_CLR_MX;
    if(pwm_counter == saved_pwm_target_value[6])
      DIM_BAND_7_CLR_MX;
  }
  else
  {      
    if(pwm_counter == saved_pwm_target_value[1])
      DIM_BAND_2_CLR_MX;
    if(pwm_counter == saved_pwm_target_value[3])
      DIM_BAND_4_CLR_MX;
    if(pwm_counter == saved_pwm_target_value[5])
      DIM_BAND_6_CLR_MX;
  }
      
  pwm_counter ++;
};


// ---------------------------------------------------------------------------------------
bool setupPWMValForIndex(uint8_t pwm_val, uint8_t index)
{
  if(index >= NUM_PWN_CHANNELS)
    return false;

  pwm_target_value[index] = pwm_val;
  
  return true;
}


// ---------------------------------------------------------------------------------------
void setupBandDimmer()
{
  // set the outputs
  pinMode(DIM_MUX, OUTPUT);
  pinMode(DIM_BAND_1, OUTPUT);
  pinMode(DIM_BAND_2_3, OUTPUT);
  pinMode(DIM_BAND_4_5, OUTPUT);
  pinMode(DIM_BAND_6_7, OUTPUT);


  values_dim_mux_hi = PORT_DIM_MASK_SET_HI;
  values_dim_mux_lo = PORT_DIM_MASK_SET_LO;

  pwm_multiplex_idx = 0;

  // init pwm
  for(int idx = 0; idx < NUM_PWN_CHANNELS; idx ++)
  {
    setupPWMValForIndex(led7brightnessOff, idx);
  }
  
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 0x00;        //interrupt every 256 ticks
  TIFR2  = 0x00;        //Timer2 clear flags
  TIMSK2 = 0x01;        //Timer2 enable overflow interrupt
  TCCR2A = 0x00;        //Timer2 normal operation
  TCCR2B = 0x01;        //Timer2 prescale = 1
}
// ---------------------------------------------------------------------------------------
// END: PWM Light band 
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// START light effects .... // Josef
// ---------------------------------------------------------------------------------------

int effect_pos = -1;

// ---------------------------------------------------------------------------------------
// effect array used by the light_effect_from_array() method
// current effect: running light
PROGMEM  prog_uchar light_effect[] =
{ 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 7, 7,
8, 9, 10, 11, 12, 13, 15, 16, 18, 20, 22, 25, 27, 30, 33, 37, 40, 45, 49, 55, 60,
67, 74, 81, 90, 99, 110, 122, 134, 148, 164, 181, 200, 221, 245,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
245, 245, 221, 221, 200, 200, 181, 181, 164, 164, 148, 148, 134, 134, 122, 122,
110, 110, 99, 99, 90, 90, 81, 81, 74, 74, 67, 67, 60, 60, 55, 55, 49, 49, 45, 45,
40, 40, 37, 37, 33, 33, 30, 30, 27, 27, 25, 25, 22, 22, 20, 20, 18, 18, 16, 16,
15, 15, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5,
5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
1, 1, 1, 1, 1, 1, 1 };


const int NUM_EFFECT_VALUES = sizeof(light_effect) / sizeof(light_effect[0]);

// ---------------------------------------------------------------------------------------
// uses the light_effect array to display effects
// param: steps: effect_speed
// ---------------------------------------------------------------------------------------
void light_effect_from_array(int steps)
{
  int min_value = 2; // residual glow
  int tmp_pos_divider = 5; // width of the effect
  int end_pos = 650 + NUM_EFFECT_VALUES * tmp_pos_divider; // 600 nearly continuous loop
  
  if((effect_pos < 0) || (effect_pos >= end_pos))
     effect_pos = 0;

  int tmp_pos;
  tmp_pos = effect_pos / tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[0] = min_value;
  else
    pwm_target_value[0] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 100)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[1] = min_value;
  else
    pwm_target_value[1] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 200)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[2] = min_value;
  else
    pwm_target_value[2] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 300)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[3] = min_value;
  else
    pwm_target_value[3] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 400)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[4] = min_value;
  else
    pwm_target_value[4] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 500)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[5] = min_value;
  else
    pwm_target_value[5] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);

  tmp_pos = (effect_pos - 600)/tmp_pos_divider;
  if((tmp_pos < 0) || (tmp_pos >= NUM_EFFECT_VALUES))
    pwm_target_value[6] = min_value;
  else
    pwm_target_value[6] = max(pgm_read_byte_near(light_effect + tmp_pos), min_value);
    
  effect_pos += steps;
}


// ---------------------------------------------------------------------------------------
// sinus wave effect
// param: steps: effect_speed
// ---------------------------------------------------------------------------------------
void loopEffect_SIN(int steps)
{
  if(effect_pos >= 800)
     effect_pos = 0;
    

   if(effect_pos <= 200)
      pwm_target_value[0] = 127.5 * (1.0 - cos((M_PI * effect_pos) /100));
    else
      pwm_target_value[0] = 0;
    
    if((effect_pos >= 100) && (effect_pos <= 300))
      pwm_target_value[1] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 100)) /100));
    else
      pwm_target_value[1] = 0;

    if((effect_pos >= 200) && (effect_pos <= 400))
      pwm_target_value[2] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 200)) /100));
    else
      pwm_target_value[2] = 0;

    if((effect_pos >= 300) && (effect_pos <= 500))
      pwm_target_value[3] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 300)) /100));
    else
      pwm_target_value[3] = 0;

    if((effect_pos >= 400) && (effect_pos <= 600))
      pwm_target_value[4] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 400)) /100));
    else
      pwm_target_value[4] = 0;

    if((effect_pos >= 500) && (effect_pos <= 700))
      pwm_target_value[5] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 500)) /100));
    else
      pwm_target_value[5] = 0;

    if((effect_pos >= 600) && (effect_pos <= 800))
      pwm_target_value[6] = 127.5 * (1.0 - cos((M_PI * (effect_pos - 600)) /100));
    else
      pwm_target_value[6] = 0;

    effect_pos += steps;
  
}

// ---------------------------------------------------------------------------------------
// fade in/fade out effect
// param: steps: effect_speed
// ---------------------------------------------------------------------------------------
void loopEffect_FADE(int steps)
{
  if(effect_pos >= 800)
    effect_pos = 0;
    
  int tgt_val = 255*(0.5 + 0.5 * sin(M_PI*effect_pos/400));
  
  memset(pwm_target_value, tgt_val, NUM_PWN_CHANNELS);
  
  effect_pos += steps;
}

// ---------------------------------------------------------------------------------------
// END light effects ....
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------



enum {
  c_lcdCharsCount = 16,
  c_lcdLinesCount = 2
};

enum {
  modeNone,
  modeWelcome,
  modeMainMenu,
  modeShutdown
};

enum {
  keyNone,
  keyOk,
  keyCancel,
  keyLeft,
  keyRight,
  keyUp,
  keyDown,
  keyUnknown
};

enum {
  keyStateOk = 0x10,
  keyStateCancel = 0x20,
  keyStateLeft = 0x02,
  keyStateRight = 0x08,
  keyStateUp = 0x04,
  keyStateDown = 0x01
};

enum {
  c_inputBufSize = 64
};

enum {
  c_channelStateInvalid,
  c_channelStateAdmDown,
  c_channelStateStarting,
  c_channelStateUp,
  c_channelStateDown,
  c_channelStateUnknown
};

enum {
  c_rebootModeUnknown,
  c_rebootModeShutdown,
  c_rebootModeReboot
};

struct PanelParams
{
  bool have_backlight;
  unsigned char backlight;
  bool have_color;
  unsigned char color[3];
  bool have_keyboard;
  unsigned char keyboard[12];
};

struct MenuInfo
{
  char **items;
  int count;
  int backIndex;
};

struct MenuState
{
  int top;
  int sel;
};

// initialize the library with the numbers of the interface pins
//DogM162 lcd(7, 4, 3, 2, 12, 8); // d4 d5 d6 d7 rs en
EAW162 lcd(7, 4, 3, 2, 10, 8); // d4 d5 d6 d7 rs en
Keyb* keyb;

int mode = modeNone;

char inputBuf[c_inputBufSize + 1];
int inputBufFill = 0;
bool haveKeepalive = false;
char lastCmd[c_inputBufSize + 1];

bool globalCancel = false;
bool disableGlobalCancel = false;
int rebootMode = c_rebootModeUnknown;

bool readPowerState()
{
  int value;
  value = analogRead(PIN_USB_POWER);
  return value > 512;
}

void cleanRxBuffer()
{
  while (Serial.available())
    Serial.read();
}

void resetKeepalive()
{
  haveKeepalive = false;
}

void resetInput()
{
  cleanRxBuffer();
  inputBufFill = 0;
  *lastCmd = 0;
}

unsigned char parseOneHex(char chr, bool *valid)
{
  if (chr >= '0' && chr <= '9')
    return chr - '0';
  if (chr >= 'a' && chr <= 'f')
    return chr - 'a' + 10;
  if (chr >= 'A' && chr <= 'F')
    return chr - 'A' + 10;
  if (valid)
    *valid = false;
  return 0;
}

void parseHex(const char *str, size_t cnt, unsigned char *val, bool *valid)
{
  size_t index;
  unsigned char digit[2];
  for ( index = 0 ; index < cnt ; index++ ) {
    digit[0] = parseOneHex(str[index * 2 + 0], valid);
    digit[1] = parseOneHex(str[index * 2 + 1], valid);
    val[index] = (digit[0] << 4) | digit[1];
  }
}

void handleParams(struct PanelParams &params)
{
 
  //setBacklight(params.backlight); // removed by Josef, no backlight anymore
  analogWrite(PIN_OUTP102_3, params.color[0]);
  analogWrite(PIN_OUTP102_4, params.color[1]);
  analogWrite(PIN_OUTP102_5, params.color[2]);  

  if (DEV_MODE)
    Serial.println(params.backlight);
    Serial.println(params.color[0]);
    Serial.println(params.color[1]);
    Serial.println(params.color[2]);
    

}

bool parseParams(const char *msg)
{
  size_t len;
  struct PanelParams params;

  if (DEV_MODE)
    Serial.println(msg);
    
  if (!msg)
    return false;

  len = strlen(msg);

//  if (len != 41 || strncmp(msg, "params/", 7) != 0) {
//    if (DEV_MODE)
//      Serial.println("Incorrect params string");
//    return false;
//  }
//
//  if (msg[6] != '/' || msg[9] != '/' || msg[16] != '/') {
//    if (DEV_MODE)
//      Serial.println("Incorrect params string");
//    return false;
//  }

  parseHex(msg + 7, 1, &params.backlight, &params.have_backlight);
  parseHex(msg + 10, 3, params.color, &params.have_color);
  parseHex(msg + 17, 12, params.keyboard, &params.have_keyboard);

  handleParams(params);

  return true;
}

void readInput()
{
  static const char * const c_prefix = "###";
  static int const c_prefixLen = 3;
  static const char * const c_suffix = "===";
  static int const c_suffixLen = 3;

  char ch;
  char *pos, *pos2;

  while (true) {

    if (!Serial.available())
      break;

    if (inputBufFill >= c_inputBufSize - 4) {
      inputBuf[inputBufFill] = 0;
      pos = strstr(inputBuf, c_prefix);
      if (pos == inputBuf)
        pos = strstr(inputBuf + 1, c_prefix);
      if (!pos) {
        inputBufFill = 0;
      } else {
        memmove(inputBuf, pos, inputBuf + inputBufFill - pos);
      }
    }

    ch = Serial.read();
    if (!ch)
      ch = ' ';

    inputBuf[inputBufFill] = ch;
    inputBufFill++;
    inputBuf[inputBufFill] = 0;

    pos = strstr(inputBuf, c_prefix);
    if (!pos)
      continue;
    pos += c_prefixLen;

    pos2 = strstr(pos, c_suffix);
    if (!pos2)
      continue;

    *pos2 = 0;

    if (strcmp(pos, "keepalive") == 0) {
      haveKeepalive = true;
    } else if (strncmp(pos, "params", 6) == 0) {
      parseParams(pos);
    } else if (strcmp(pos, "user:shutdown") == 0) {
      if (!disableGlobalCancel) {
        globalCancel = true;
        mode = modeShutdown;
        rebootMode = c_rebootModeShutdown;
      }
    } else if (strcmp(pos, "user:reboot") == 0) {
      if (!disableGlobalCancel) {
        globalCancel = true;
        mode = modeShutdown;
        rebootMode = c_rebootModeReboot;
      }
    } else if (strcmp(pos, "user:unknown-halt") == 0) {
      if (!disableGlobalCancel) {
        globalCancel = true;
        mode = modeShutdown;
        rebootMode = c_rebootModeUnknown;
      }
    } else {
      strcpy(lastCmd, pos);
    }

    pos2 += c_suffixLen;
    inputBufFill = inputBuf + inputBufFill - pos2;
    memmove(inputBuf, pos2, inputBufFill);
  }
}

void ourDelay(unsigned long time)
{
  unsigned long quant;

  quant = 1;

  while (time) {

    if (globalCancel)
      break;

    readInput();

    if (time < quant)
      quant = time;

    delay(quant);
    time -= quant;
  }

  readInput();
}

void pressPower()
{
  digitalWrite(PIN_SERVER_POWER, HIGH);
}

void releasePower()
{
  digitalWrite(PIN_SERVER_POWER, LOW);
}

void longPressPower(bool check)
{
  if (globalCancel)
    return;

  pressPower();

  if (!check) {
    ourDelay(6000);
  } else {
    while (readPowerState()) {
      if (globalCancel)
        break;
    }
  }

  releasePower();
}

void shortPressPower()
{
  if (globalCancel)
    return;

  pressPower();
  ourDelay(500);
  releasePower();
}

void emergencyTurnOff()
{
  longPressPower(false);
  ourDelay(1000);
}

void printText(const char *text)
{
  lcd.write(text);
}

void printTextLine(const char *text, int row)
{
  if (!text)
    return;
  lcd.SetPosition(0, row);
  lcd.write(text);
}

void setText(const char *line1, const char *line2 = 0)
{
  lcd.Clear();

  if (line1) {
    lcd.SetPosition(0, 0);
    lcd.write(line1);
  }

  if (line2) {
    lcd.SetPosition(0, 1);
    lcd.write(line2);
  }

  //lcd.display();
}


void setText(const __FlashStringHelper *line1, const __FlashStringHelper *line2 = 0)
{
  lcd.Clear();

  if (line1) {
    lcd.SetPosition(0, 0);
    lcd.print(line1);
  }

  if (line2) {
    lcd.SetPosition(0, 1);
    lcd.print(line2);
  }

  //lcd.display();
}


void clearText()
{
  lcd.Clear();
}

void showText()
{
  //lcd.display();
}

void showMsg(int time, const __FlashStringHelper *line1, const __FlashStringHelper *line2 = 0)
{
  if (globalCancel)
    return;

  setText(line1, line2);
  ourDelay(time);
  clearText();
}

void afterMsg(int time)
{
  if (globalCancel)
    return;

  if (time)
    ourDelay(time);
  clearText();
}

uint8_t readSpiState()
{
  uint8_t state(0xFF);
  char key_data[2];

  if (!keyb->read(key_data))
    return state;

  if (key_data[1] & 0x02) {
    state &= ~keyStateUp;
    //m_backlight_brightness += 5;
    //setBacklight(m_backlight_brightness);
  }
  if (key_data[0] & 0x01)
    state &= ~keyStateLeft;
  if (key_data[0] & 0x08) {
    state &= ~keyStateDown;
    //m_backlight_brightness -= 5;
    //setBacklight(m_backlight_brightness);
  }
  if (key_data[1] & 0x08)
    state &= ~keyStateRight;
  if (key_data[1] & 0x04)
    state &= ~keyStateOk;
  if (key_data[0] & 0x04)
    state &= ~keyStateCancel;

  return state;
}

int waitForKey(int time)
{
  int quant;
  uint8_t state;
  int key;

  quant = 50;
  key = keyNone;

  while (time > 0) {

    if (globalCancel)
      return keyNone;

    state = readSpiState();

    if (!(state & keyStateDown)) {
      key = keyDown;
    } else if (!(state & keyStateLeft)) {
      key = keyLeft;
    } else if (!(state & keyStateUp)) {
      key = keyUp;
    } else if (!(state & keyStateRight)) {
      key = keyRight;
    } else if (!(state & keyStateOk)) {
      key = keyOk;
    } else if (!(state & keyStateCancel)) {
      key = keyCancel;
    }

    if (key != keyNone)
      break;

    ourDelay(quant);
    time -= quant;
  }

  return key;
}

bool waitForKeyUp(int key, int time)
{
  int quant;
  uint8_t state;
  uint8_t mask;
  bool res;

  switch (key)
  {
    case keyDown:
      mask = keyStateDown;
      break;
    case keyLeft:
      mask = keyStateLeft;
      break;
    case keyUp:
      mask = keyStateUp;
      break;
    case keyRight:
      mask = keyStateRight;
      break;
    case keyOk:
      mask = keyStateOk;
      break;
    case keyCancel:
      mask = keyStateCancel;
      break;
    default:
      mask = 0;
      break;
  }

  if (!mask)
    return true;

  quant = 50;
  res = false;

  while (time > 0) {

    if (globalCancel)
      return false;

    state = readSpiState();

    if (state & mask) {
      res = true;
      break;
    }

    ourDelay(quant);
    time -= quant;
  }

  return res;
}

void turnOnLed()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
}

void turnOffLed()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}
/*
// removed by Josef, no backlight anymore
void setBacklight(uint8_t brightness)
{
  analogWrite(PIN_BACKLIGHT, brightness);
}
*/


void set7Leds(uint8_t brightness)
{
//  analogWrite(PIN_7LEDS, brightness); // Josef : no PIN_7LEDS pin anymore
  
  for(int idx = 0; idx < NUM_PWN_CHANNELS; idx ++)
  {
    setupPWMValForIndex(brightness, idx);
  }

}

void waitForKeepalive(bool exitOnPowerOff)
{
  resetKeepalive();

  while (!haveKeepalive) {
    
    // Josef
    // possible loop effects for booting ....
    // only one works at a time!!!!!
      light_effect_from_array(5);
//    loopEffect_SIN(3);
//    loopEffect_FADE(2);

    if (globalCancel)
      return;

    if (exitOnPowerOff && !readPowerState())
      break;

    ourDelay(10); // Josef: changed from 100 to have a smooth running light
  }
}

bool waitForReply()
{
  unsigned long time;
  int quant;
  int keepaliveCount;
  
  quant = 10;
  time = 0;

  resetKeepalive();
  keepaliveCount = 0;

  while (true) {

    if (globalCancel)
      return false;

#if DEV_MODE

    // do nothing

#else

    if (!readPowerState())
      return false;

#endif

    if (*lastCmd)
      break;

    if (haveKeepalive) {
      keepaliveCount++;
      resetKeepalive();
    }

    if (time > 5000) {
      if (keepaliveCount < 2)
        return false;
      time = 0;
      keepaliveCount = 0;
    }

    ourDelay(quant);
    time += quant;
  }

  return true;
}

void drawMenu(struct MenuInfo &info, struct MenuState &state)
{
  int index, fullIndex, numIndex;
  char prefix[32];
  int pos;

  if (globalCancel)
    return;

  clearText();

  for ( index = 0 ; index < c_lcdLinesCount ; index++ ) {

    fullIndex = state.top + index;
    numIndex = fullIndex + 1;

    if (fullIndex >= info.count)
      break;

    prefix[0] = (fullIndex == state.sel) ? '>' : ' ';

    if (numIndex >= 10) {
      prefix[1] = '0' + numIndex / 10;
      prefix[2] = '0' + numIndex % 10;
      pos = 3;
    } else {
      prefix[1] = '0' + numIndex % 10;
      pos = 2;
    }

    prefix[pos + 0] = '.';
    prefix[pos + 1] = 0;

    printTextLine(prefix, index);
    printText(info.items[fullIndex]);
  }

  showText();
}

void initMenuState(struct MenuState &state)
{
  state.top = 0;
  state.sel = 0;
}

int menuFunc(struct MenuInfo &info, struct MenuState &state)
{
  int key;
  int res;
  bool changed;

  res = -1;

  while (res < 0) {

    if (globalCancel)
      return -1;

    drawMenu(info, state);

    changed = false;
    while (!changed) {

      if (globalCancel)
        return -1;
        
      key = waitForKey(1000);
      waitForKeyUp(key, 1000);

      switch (key)
      {

        case keyUp:
          if (state.sel <= 0)
            break;
          state.sel--;
          changed = true;
          break;

        case keyDown:
          if (state.sel + 1 >= info.count)
            break;
          state.sel++;
          changed = true;
          break;

        case keyOk:
          res = state.sel;
          changed = true;
          break;

        case keyCancel:
          if (info.backIndex == -2) {
            res = info.count - 1;
          } else if (info.backIndex < 0) {
            break;
          } else {
            res = info.backIndex;
          }
          changed = true;
          break;

      }

      if (state.sel < state.top) {
        state.top = state.sel;
        changed = true;
      }

      if (state.sel >= state.top + c_lcdLinesCount) {
        state.top = state.sel - c_lcdLinesCount + 1;
        changed = true;
      }

    }

  }

  return res;
}

bool yesNoMsgExt(const char *textStr, const char *yesStr, const char *noStr, bool exitOnPwrOff, bool exitOnPwrOn, bool *pwrState)
{
  bool pwr;
  const char *prefix;
  bool sel;
  bool changed;
  bool finished;
  int key;

  sel = false;
  finished = false;

  while (!finished) {

    if (globalCancel)
      return false;

    clearText();
    printTextLine(textStr, 0);

    prefix = sel ? ">" : " ";
    printTextLine(prefix, 1);
    printText(yesStr);

    printText(" ");

    prefix = !sel ? ">" : " ";
    printText(prefix);
    printText(noStr);

    changed = false;
    while (!changed) {

      if (globalCancel)
        return false;

      pwr = readPowerState();
      if (pwrState)
        *pwrState = pwr;

      if (exitOnPwrOff && !pwr || exitOnPwrOn && pwr)
        return false;

      key = waitForKey(200);
      waitForKeyUp(key, 1000);

      switch (key)
      {

        case keyLeft:
          if (sel)
            break;
          sel = true;
          changed = true;
          break;

        case keyRight:
          if (!sel)
            break;
          sel = false;
          changed = true;
          break;

        case keyOk:
          changed = true;
          finished = true;
          break;

      }

    }
  }

  return sel;
}

bool yesNoMsg(const char *textStr, const char *yesStr, const char *noStr)
{
  return yesNoMsgExt(textStr, yesStr, noStr, false, false, 0);
}

bool ipAddrDialog(int &a0, int &a1, int &a2, int &a3, int &m, bool &d, bool mask, bool dhcp)
{
  bool none;
  bool finished;
  bool changed, moved;
  int pos, subPos;
  char line[32];
  int *curr;
  int mul, maxVal, minVal;
  bool isBlink;
  int key;

  none = a0 < 0 || a1 < 0 || a2 < 0 || a3 < 0 || (mask && m < 0);

  pos = 0;
  subPos = 0;

  if (dhcp && d) {
    pos = 5;
    subPos = 0;
  }

  finished = false;
  isBlink = false;

  while (!finished) {

    if (globalCancel)
      return false;

    if (pos == 0) {
      curr = &a0;
    } else if (pos == 1) {
      curr = &a1;
    } else if (pos == 2) {
      curr = &a2;
    } else if (pos == 3) {
      curr = &a3;
    } else if (pos == 4) {
      curr = &m;
    } else {
      curr = 0;
    }

    if (pos >= 0 && pos <= 3) {
      if (subPos == 0) {
        mul = 100;
      } else if (subPos == 1) {
        mul = 10;
      } else if (subPos == 2) {
        mul = 1;
      } else {
        mul = 0;
      }
      maxVal = 255;
      minVal = 0;
    } else if (pos == 4) {
      if (subPos == 0) {
        mul = 10;
      } else if (subPos == 1) {
        mul = 1;
      } else {
        mul = 0;
      }
      maxVal = 32;
      minVal = 0;
    } else {
      mul = 0;
      maxVal = 0;
      minVal = 0;
    }

    clearText();

    if (none) {
      strcpy_P(line, PSTR("---.---.---.---"));
    } else {
      sprintf_P(line, PSTR("%3i.%3i.%3i.%3i"), a0, a1, a2, a3);
    }
    if (isBlink && pos >= 0 && pos <= 3 && subPos >= 0 && subPos <= 2)
      line[pos * 4 + subPos] = '#';
    printTextLine(line, 0);

    if (mask) {
      if (none) {
        strcpy(line, "/--");
      } else {
        sprintf(line, "/%2i", m);
      }
      if (isBlink && pos == 4 && subPos >= 0 && subPos <= 1)
        line[1 + subPos] = '#';
      printTextLine(line, 1);
      printText("  ");
    } else {
      printTextLine("", 1);
    }

    if (dhcp) {
      if (pos == 5) {
        strcpy(line, ">");
      } else {
        strcpy(line, " ");
      }
      if (isBlink && pos == 5) {
        strcat(line, ">");
      } else {
        strcat(line, " ");
      }
      if (d) {
        strcat(line, "DHCP");
      } else {
        strcat(line, "NO DHCP");
      }
      printText(line);
    }

    key = waitForKey(isBlink ? 500 : 500);
    waitForKeyUp(key, 1000);

    changed = false;
    moved = false;

    switch (key)
    {

      case keyLeft:
        if (dhcp && d && pos == 5) {
          break;
        } else if (pos == 0 && subPos == 0) {
          break;
        } else if (pos == 5 && mask) {
          pos = 4;
          subPos = 1;
        } else if (pos == 5 && !mask) {
          pos = 3;
          subPos = 2;
        } else if (subPos > 0) {
          subPos--;
        } else {
          pos--;
          subPos = 2;
        }
        moved = true;
        break;

      case keyRight:
        if (pos == 5) {
          break;
        } else if (pos == 4 && subPos == 1) {
          if (dhcp) {
            pos = 5;
            subPos = 0;
          } else {
            break;
          }
        } else if (!mask && pos == 3 && subPos == 2) {
          if (dhcp) {
            pos = 5;
            subPos = 0;
          } else {
            break;
          }
        } else if (pos < 4 && subPos == 2) {
          pos++;
          subPos = 0;
        } else {
          subPos++;
        }
        moved = true;
        break;

      case keyUp:
      {
        int left, right, digit;
        if (pos == 5) {
          d = !d;
          changed = true;
          break;
        }
        if (!curr || !mul)
          break;
        if (none) {
          a0 = 0;
          a1 = 0;
          a2 = 0;
          a3 = 0;
          m = pos == 4 ? 0 : 24;
          none = false;
        }
        left = *curr / mul / 10;
        right = *curr % mul;
        digit = *curr / mul % 10;
        digit++;
        *curr = left * mul * 10 + digit * mul + right;
        if (*curr > maxVal)
          *curr = maxVal;
        changed = true;
        break;
      }

      case keyDown:
      {
        int left, right, digit;
        if (pos == 5) {
          d = !d;
          changed = true;
          break;
        }
        if (!curr || !mul)
          break;
        if (none) {
          a0 = 0;
          a1 = 0;
          a2 = 0;
          a3 = 0;
          m = pos == 4 ? 0 : 24;
          none = false;
        }
        left = *curr / mul / 10;
        right = *curr % mul;
        digit = *curr / mul % 10;
        if (digit == 0 && left == 0)
          break;
        digit--;
        *curr = left * mul * 10 + digit * mul + right;
        if (*curr < minVal)
          *curr = minVal;
        changed = true;
        break;
      }

      case keyOk:
        changed = true;
        finished = true;
        break;

      case keyCancel:
        changed = false;
        finished = true;
        break;

    }

    if (changed) {
      isBlink = false;
    } else if (moved) {
      isBlink = true;
    } else {
      isBlink = !isBlink;
    }
  }

  return changed;
}

void actionPowerOn()
{
  bool res;

  if (readPowerState()) {
    showMsg(2000, F("Not available"), F("Already pwr on"));
    return;
  }

  res = yesNoMsg("Power on?", "yes", "no");
  if (!res) {
    afterMsg(0);
    return;
  }

  if (readPowerState()) {
    showMsg(2000, F("Not available"), F("Already pwr on"));
    return;
  }

  if (globalCancel)
    return;

  setText(F("Powering on..."));
  shortPressPower();

#if DEV_MODE

  if (readPowerState()) {
    set7Leds(led7brightnessOn);
    setText(F("Booting..."));
  }
  
  waitForKeepalive(false);

#else

  while (!readPowerState()) {
    if (globalCancel)
      return;
    ourDelay(100);
  }

  if (readPowerState()) { 
    set7Leds(led7brightnessOn);
    setText(F("Booting..."));
  }
  waitForKeepalive(true);

  if (!readPowerState()) {
    showMsg(3000, F("Unexpected"), F("power off"));
    set7Leds(led7brightnessOff);
    return;
  }

#endif

  afterMsg(500);
}

void shutdownCommon(bool reboot)
{
  bool res;
  int quant;
  unsigned long timeout;
  int keepaliveCount;

  if (reboot) {
    setText(F("Rebooting..."));
  } else {
    setText(F("Shutting down..."));
  }

  if (reboot) {
    resetKeepalive();
    keepaliveCount = 0;
  }

  timeout = 5 * 60 * 1000;
  quant = 100;

  if (reboot)
    timeout = timeout * 3 / 2;

  while (readPowerState()) {

    if (globalCancel)
      return;

    if (reboot) {
      if (haveKeepalive) {
        keepaliveCount++;
        resetKeepalive();
      }
      if (keepaliveCount > 5)
        return;
    }

    if (timeout <= 0) {
      res = yesNoMsgExt("Force off?", "yes", "no", true, false, 0);
      afterMsg(0);
      if (res)
        break;
    }

    ourDelay(quant);
    timeout -= quant; 
  }

  if (readPowerState()) {
    setText(F("Forcing off..."));
    longPressPower(true);
  }

  afterMsg(500);
  set7Leds(led7brightnessOff);
}

void actionPowerOff()
{
  bool res;

  if (!readPowerState()) {
    showMsg(2000, F("Not available"), F("Already pwr off"));
    return;
  }

  res = yesNoMsg("Power off?", "yes", "no");
  if (!res) {
    afterMsg(0);
    return;
  }

  if (!readPowerState()) {
    showMsg(2000, F("Not available"), F("Already pwr off"));
    return;
  }

  if (globalCancel)
    return;

  disableGlobalCancel = true;
  globalCancel = false;

  setText(F("Powering off..."));
  shortPressPower();

  shutdownCommon(false);

  globalCancel = false;
  disableGlobalCancel = false;
}

bool sendHardReset()
{
  resetInput();
  Serial.print(F("###hardreset==="));

  if (!waitForReply())
    return false;

  if (strcmp(lastCmd, "ok") == 0)
    return true;

  return false;
}

void actionHardReset()
{
  bool res;

  if (!readPowerState()) {
    showMsg(2000, F("Not available"), F("System not booted"));
    return;
  }

  res = yesNoMsg("Reset config?", "yes", "no");
  if (!res) {
    afterMsg(0);
    return;
  }

  if (!readPowerState()) {
    showMsg(2000, F("Not available"), F("System not booted"));
    return;
  }

  if (globalCancel)
    return;

  setText(F("Resetting..."));

  res = sendHardReset();
  if (!res) {
    showMsg(2000, F("Failed to reset"), F("system config"));
    return;
  }

  clearText();
}

bool setIpAddress(int a0, int a1, int a2, int a3, int m, bool d)
{
  char msg[64];

  if (d) {
    strcpy_P(msg, PSTR("###ipaddr/dhcp==="));
  } else {
    sprintf_P(msg, PSTR("###ipaddr/%3i.%3i.%3i.%3i/%2i==="), a0, a1, a2, a3, m);
  }

  resetInput();
  Serial.print(msg);

  if (!waitForReply())
    return false;

  if (strcmp(lastCmd, "ok") == 0)
    return true;

  return false;
}

bool queryIpAddress(int &a0, int &a1, int &a2, int &a3, int &m, bool &d)
{
  char msg[c_inputBufSize + 1];
  int len;
  char *end;
  bool res;

  resetInput();
  Serial.print(F("###getdhcp==="));

  if (!waitForReply())
    return false;

  strcpy(msg, lastCmd);
  if (strcmp(msg, "dhcp/1") == 0) {
    d = true;
  } else if (strcmp(msg, "dhcp/0") == 0) {
    d = false;
  } else {
    return false;
  }

  resetInput();
  Serial.print(F("###getip==="));

  if (!waitForReply())
    return false;

  strcpy(msg, lastCmd);
  len = strlen(msg);

  if (strcmp(msg, "ip/none") == 0) {
    a0 = -1;
    a1 = -1;
    a2 = -1;
    a3 = -1;
    m = -1;
    return true;
  }

  if (len != 21 || strncmp(msg, "ip/", 3) != 0)
    return false;

  if (msg[6] != '.' || msg[10] != '.' || msg[14] != '.' || msg[18] != '/')
    return false;

  res = true;

  if (res) {
    end = 0;
    a0 = strtoul(msg + 3, &end, 10);
    res = end == msg + 6;
  }

  if (res) {
    end = 0;
    a1 = strtoul(msg + 7, &end, 10);
    res = end == msg + 10;
  }

  if (res) {
    end = 0;
    a2 = strtoul(msg + 11, &end, 10);
    res = end == msg + 14;
  }

  if (res) {
    end = 0;
    a3 = strtoul(msg + 15, &end, 10);
    res = end == msg + 18;
  }

  if (res) {
    end = 0;
    m = strtoul(msg + 19, &end, 10);
    res = end && !*end;
  }

  if (!res)
    return false;

  return true;
}

void actionConfIpAddress()
{
  bool res;
  int a0, a1, a2, a3, m;
  bool d;

  setText(F("Querying..."));

  res = queryIpAddress(a0, a1, a2, a3, m, d);
  if (!res) {
    showMsg(2000, F("Failed to query"), F("curr IP address"));
    return;
  }

  if (globalCancel)
    return;

  clearText();

  res = ipAddrDialog(a0, a1, a2, a3, m, d, true, true);
  if (!res)
    return;

  if (globalCancel)
    return;

  setText(F("Saving..."));

  res = setIpAddress(a0, a1, a2, a3, m, d);
  if (!res) {
    showMsg(2000, F("Failed to set"), F("new IP address"));
    return;
  }

  clearText();
}

bool setGatewayAddress(int a0, int a1, int a2, int a3)
{
  char msg[64];

  sprintf_P(msg, PSTR("###gateway/%3i.%3i.%3i.%3i==="), a0, a1, a2, a3);

  resetInput();
  Serial.print(msg);

  if (!waitForReply())
    return false;

  if (strcmp(lastCmd, "ok") == 0)
    return true;

  return false;
}

bool queryGatewayAddress(int &a0, int &a1, int &a2, int &a3)
{
  char msg[c_inputBufSize + 1];
  int len;
  char *end;
  bool res;

  resetInput();
  Serial.print(F("###getgateway==="));

  if (!waitForReply())
    return false;

  strcpy(msg, lastCmd);
  len = strlen(msg);

  if (strcmp(msg, "gateway/none") == 0) {
    a0 = -1;
    a1 = -1;
    a2 = -1;
    a3 = -1;
    return true;
  }

  if (len != 23 || strncmp(msg, "gateway/", 8) != 0)
    return false;

  if (msg[11] != '.' || msg[15] != '.' || msg[19] != '.')
    return false;

  res = true;

  if (res) {
    end = 0;
    a0 = strtoul(msg + 8, &end, 10);
    res = end == msg + 11;
  }

  if (res) {
    end = 0;
    a1 = strtoul(msg + 12, &end, 10);
    res = end == msg + 15;
  }

  if (res) {
    end = 0;
    a2 = strtoul(msg + 16, &end, 10);
    res = end == msg + 19;
  }

  if (res) {
    end = 0;
    a3 = strtoul(msg + 20, &end, 10);
    res = end && !*end;
  }

  if (!res)
    return false;

  return true;
}

void actionConfGatewayAddress()
{
  bool res;
  int a0, a1, a2, a3, m;
  bool d;

  setText(F("Querying..."));

  res = queryGatewayAddress(a0, a1, a2, a3);
  if (!res) {
    showMsg(2000, F("Failed to query"), F("curr GW address"));
    return;
  }

  if (globalCancel)
    return;

  clearText();

  m = -1;
  d = false;
  res = ipAddrDialog(a0, a1, a2, a3, m, d, false, false);
  if (!res)
    return;

  if (globalCancel)
    return;

  setText(F("Saving..."));

  res = setGatewayAddress(a0, a1, a2, a3);
  if (!res) {
    showMsg(2000, F("Failed to set"), F("new GW address"));
    return;
  }

  clearText();
}

/*
bool queryUptime(char *t, char *&a1, char *&a2, char *&a3)
{
  int len;

  a1 = 0;
  a2 = 0;
  a3 = 0;

  resetInput();
  Serial.print(F("###getuptime==="));

  if (!waitForReply())
    return false;

  strcpy(t, lastCmd);
  len = strlen(t);

  if (len < 7 || strncmp(t, "uptime/", 7) != 0)
    return false;

  len -= 7;
  memmove(t, t + 7, len);
  t[len] = 0;

  a1 = strchr(t, '/');
  if (!a1)
    return false;
  *a1 = 0;
  a1++;

  a2 = strchr(a1, '/');
  if (!a2)
    return false;
  *a2 = 0;
  a2++;

  a3 = strchr(a2, '/');
  if (!a3)
    return false;
  *a3 = 0;
  a3++;

  return true;
}

void actionShowUptime()
{
  bool res;
  char t[c_inputBufSize + 1];
  char *a1, *a2, *a3;
  int key;
  bool back;
  bool reload;

  back = false;

  while (!back) {

    if (globalCancel)
      return;

    setText(F("Querying..."));

    res = queryUptime(t, a1, a2, a3);
    if (!res) {
      showMsg(2000, F("Failed to query"), F("server uptime"));
      return;
    }

    if (globalCancel)
      return;

    clearText();

    printTextLine("Uptime: ", 0);
    printText(t);

    printTextLine(a1, 1);
    printText(" ");
    printText(a2);
    printText(" ");
    printText(a3);

    reload = false;

    while (!back && !reload) {

      if (globalCancel)
        return;

      key = waitForKey(1000);
      waitForKeyUp(key, 1000);

      switch (key)
      {
        case keyOk:
        case keyCancel:
          back = true;
          break;
        case keyUp:
        case keyDown:
        case keyLeft:
        case keyRight:
          reload = true;
          break;
      }

    }

  }

  clearText();
}
*/

bool queryLoad(char *t)
{
  int len;

  *t = 0;

  resetInput();
  Serial.print(F("###getload==="));

  if (!waitForReply())
    return false;

  strcpy(t, lastCmd);
  len = strlen(t);

  if (len < 5 || strncmp(t, "load/", 5) != 0)
    return false;

  len -= 5;
  memmove(t, t + 5, len);
  t[len] = 0;

  return true;
}

void actionShowLoad()
{
  bool res;
  char t[c_inputBufSize + 1];
  int key;
  bool back;
  bool reload;

  back = false;

  while (!back) {

    if (globalCancel)
      return;

    setText(F("Querying..."));

    res = queryLoad(t);
    if (!res) {
      showMsg(2000, F("Failed to query"), F("server load"));
      return;
    }

    if (globalCancel)
      return;

    clearText();

    printTextLine("Load: ", 0);
    printText(t);
    printText("%");

    reload = false;

    while (!back && !reload) {

      if (globalCancel)
        return;

      key = waitForKey(1000);
      waitForKeyUp(key, 1000);

      switch (key)
      {
        case keyOk:
        case keyCancel:
          back = true;
          break;
        case keyUp:
        case keyDown:
        case keyLeft:
        case keyRight:
          reload = true;
          break;
      }

    }

  }

  clearText();
}

bool queryChannels(int &count)
{
  char msg[c_inputBufSize + 1];
  int len;
  char *endPtr;
  bool res;

  count = 0;

  resetInput();
  Serial.print(F("###getchannels==="));

  if (!waitForReply())
    return false;

  strcpy(msg, lastCmd);
  len = strlen(msg);

  if (len < 9 || strncmp(msg, "channels/", 9) != 0)
    return false;

  endPtr = 0;
  count = strtoul(msg + 9, &endPtr, 10);
  res = endPtr && !*endPtr;

  if (!res)
    return false;

  return true;
}

bool queryChannel(int index, int &id, int &state)
{
  char msg[c_inputBufSize + 1];
  int len;
  char *txt;
  bool res;

  id = 0;
  state = c_channelStateInvalid;

  sprintf_P(msg, PSTR("###getchannel/%i==="), index);

  resetInput();
  Serial.print(msg);

  if (!waitForReply())
    return false;

  strcpy(msg, lastCmd);
  len = strlen(msg);

  if (len < 8 || strncmp(msg, "channel/", 8) != 0)
    return false;

  txt = 0;
  id = strtoul(msg + 8, &txt, 10);
  res = txt && *txt == '/';

  if (!res)
    return false;

  txt++;

  if (strcmp(txt, "ADMINISTRATIVELY DOWN") == 0) {
    state = c_channelStateAdmDown;
  } else if (strcmp(txt, "STARTING") == 0) {
    state = c_channelStateStarting;
  } else if (strcmp(txt, "UP") == 0) {
    state = c_channelStateUp;
  } else if (strcmp(txt, "DOWN") == 0) {
    state = c_channelStateDown;
  } else if (strcmp(txt, "UNKNOWN") == 0) {
    state = c_channelStateUnknown;
  } else if (strcmp(txt, "INVALID") == 0) {
    state = c_channelStateInvalid;
  } else {
    return false;
  }

  return true;
}

void actionShowChannelsList()
{
  bool res;
  bool back, reload, redraw;
  int top, sel;
  int count;
  int ids[c_lcdLinesCount];
  int state[c_lcdLinesCount];
  int index;
  const char *st;
  char msg[24];
  int key;

  top = 0;
  sel = 0;

  back = false;

  while (!back) {

    if (globalCancel)
      return;

    setText(F("Querying..."));

    res = queryChannels(count);
    if (!res) {
      showMsg(2000, F("Failed to query"), F("channels count"));
      return;
    }

    if (globalCancel)
      return;

    if (sel >= count)
      sel = count - 1;
    if (sel < 0)
      sel = 0;

    if (sel < top)
      top = sel;
    if (sel >= top + c_lcdLinesCount)
      top = sel - c_lcdLinesCount + 1;

    if (top + c_lcdLinesCount > count)
      top = count - c_lcdLinesCount;
    if (top < 0)
      top = 0;

    for ( index = 0 ; index < c_lcdLinesCount ; index++ ) {

      if (globalCancel)
        return;

      if (top + index >= count) {
        ids[index] = 0;
        state[index] = c_channelStateInvalid;
        continue;
      }

      res = queryChannel(top + index, ids[index], state[index]);
      if (!res) {
        showMsg(2000, F("Failed to query"), F("channel state"));
        return;
      }

    }

    if (globalCancel)
      return;

    reload = false;

    while (!back && !reload) {

      if (globalCancel)
        return;

      clearText();

      for ( index = 0 ; index < c_lcdLinesCount ; index++ ) {

        if (top + index >= count)
          break;

        switch (state[index])
        {
          case c_channelStateAdmDown:
            st = "ADM. DOWN";
            break;
          case c_channelStateStarting:
            st = "STARTING";
            break;
          case c_channelStateUp:
            st = "UP";
            break;
          case c_channelStateDown:
            st = "DOWN";
            break;
          case c_channelStateUnknown:
            st = "UNKNOWN";
            break;
          default:
            st = "INVALID";
            break;
        }

        sprintf_P(msg, PSTR("%c%i: %s"), top + index == sel ? '>' : ' ', ids[index], st);
        printTextLine(msg, index);
      }

      redraw = false;

      while (!back && !reload && !redraw) {

        if (globalCancel)
          return;

        key = waitForKey(1000);
        waitForKeyUp(key, 1000);

        switch (key)
        {
          case keyOk:
            reload = true;
            break;
          case keyCancel:
            back = true;
            break;
          case keyLeft:
          case keyRight:
            reload = true;
            break;
          case keyUp:
            if (sel <= 0)
              break;
            sel--;
            if (sel < top) {
              top = sel;
              reload = true;
            }
            redraw = true;
            break;
          case keyDown:
            if (sel >= count - 1)
              break;
            sel++;
            if (sel >= top + c_lcdLinesCount) {
              top = sel - c_lcdLinesCount + 1;
              reload = true;
            }
            redraw = true;
            break;
        }

      }

    }

  }

  clearText();
}

void actionShowChannels()
{
  bool res;
  int count;
  char msg[32];
  int key;
  bool back;
  bool reload;

  back = false;

  while (!back) {

    if (globalCancel)
      return;

    setText(F("Querying..."));

    res = queryChannels(count);
    if (!res) {
      showMsg(2000, F("Failed to query"), F("channels count"));
      return;
    }

    if (globalCancel)
      return;

    clearText();

    sprintf_P(msg, PSTR(" Channels: %i"), count);
    setText(msg, ">List");

    reload = false;

    while (!back && !reload) {

      if (globalCancel)
        return;

      key = waitForKey(1000);
      waitForKeyUp(key, 1000);

      switch (key)
      {
        case keyOk:
          actionShowChannelsList();
          reload = true;
          break;
        case keyCancel:
          back = true;
          break;
        case keyUp:
        case keyDown:
        case keyLeft:
        case keyRight:
          reload = true;
          break;
      }

    }

  }

  clearText();
}

void actionShowVersion()
{
  bool res;
  int count;
  char msg[32];
  int key;
  bool back;
  bool reload;

  back = false;

  while (!back) {

    if (globalCancel)
      return;

    sprintf_P(msg,PSTR("FW Ver. %s"), FW_VERSION);

    setText(msg);

    if (globalCancel)
      return;

    reload = false;

    while (!back && !reload) {

      if (globalCancel)
        return;

      key = waitForKey(1000);
      waitForKeyUp(key, 1000);

      switch (key)
      {
        case keyOk:
          back = true;
          break;
        case keyCancel:
          back = true;
          break;
        case keyUp:
        case keyDown:
        case keyLeft:
        case keyRight:
          back = true;
          break;
      }

    }

  }

  clearText();
}


void welcomeFunc()
{
  int key;

  while (true) {

    if (globalCancel)
      return;

    setText(F("Welcome to SPB"), F("TV transcoder"));
    
    key = waitForKey(2000);
    waitForKeyUp(key, 1000);
    if (key != keyNone)
      break;

    if (globalCancel)
      return;

    setText(F("Press any key"), F("to continue"));

    key = waitForKey(2000);
    waitForKeyUp(key, 1000);
    if (key != keyNone)
      break;
  }

  clearText();

  if (globalCancel)
    return;

  mode = modeMainMenu;
}

void configureMenuFunc()
{
  char *items[] = {
    "IP address",
    "Def. gateway",
    "Back"
  };
  struct MenuInfo menu = {
    items,
    sizeof(items) / sizeof(items[0]),
    -2
  };

  struct MenuState state;
  int res;
  bool back;

  initMenuState(state);
  back = false;

  while (!back) {

    if (globalCancel)
      return;

    res = menuFunc(menu, state);
    switch (res)
    {

      case 0:
      {
        actionConfIpAddress();
        break;
      }

      case 1:
        actionConfGatewayAddress();
        break;

      case 2:
        back = true;
        break;

    }
  }

  clearText();
}

void statusMenuFunc()
{
  char *items[] = {
    "Load",
    "Channels",
    "Version",
    "Back"
  };
  struct MenuInfo menu = {
    items,
    sizeof(items) / sizeof(items[0]),
    -2
  };

  struct MenuState state;
  int res;
  bool back;

  initMenuState(state);
  back = false;

  while (!back) {

    if (globalCancel)
      return;

    res = menuFunc(menu, state);
    switch (res)
    {

      case 0:
      {
        actionShowLoad();
        break;
      }

      case 1:
        actionShowChannels();
        break;

      case 2:
        actionShowVersion();
        break;

      case 3:
        back = true;
        break;

    }
  }

  clearText();
}

void mainMenuFunc()
{
  char *items[] = {
    "Power on",
    "Power off",
    "Net Config",
    "Status",
    "Reset Config"
  };

  struct MenuInfo menu = {
    items,
    sizeof(items) / sizeof(items[0]),
    -1
  };

  struct MenuState state;
  int res;
  int nextMode;

  initMenuState(state);
  nextMode = modeNone;
  
  set7Leds(led7brightnessOn); // Josef, not booting anymore, so light on

  while (true) {

    if (globalCancel)
      return;

    res = menuFunc(menu, state);
    switch (res)
    {

      case 0:
        actionPowerOn();
        break;

      case 1:
        actionPowerOff();
        break;

      case 2:
        configureMenuFunc();
        break;

      case 3:
        statusMenuFunc();
        break;

      case 4:
        actionHardReset();
        break;

      default:
        // do nothing
        break;

    }

    if (nextMode != modeNone)
      break;

  }

  clearText();

  if (globalCancel)
    return;

  mode = nextMode;
}

void shutdownFunc()
{
  bool res;

  disableGlobalCancel = true;
  globalCancel = false;

  res = true;

  switch (rebootMode)
  {
    case c_rebootModeShutdown:
      shutdownCommon(false);
      break;
    case c_rebootModeReboot:
      shutdownCommon(true);
      break;
    case c_rebootModeUnknown:
    default:
      res = false;
      break;
  }

  globalCancel = false;
  rebootMode = c_rebootModeUnknown;
  disableGlobalCancel = false;

  if (res) {
    mode = modeMainMenu;
  } else {
    mode = modeNone;
  }
}

void initialMode()
{
  turnOffLed();

  resetInput();
  resetKeepalive();

  clearText();

  globalCancel = false;
  rebootMode = c_rebootModeUnknown;
  disableGlobalCancel = false;
  mode = modeWelcome;
}

void unknownModeFunc()
{
  setText(F("Unexpected state"), F("Shutting down..."));
  turnOffLed();

  emergencyTurnOff();
  initialMode();
}

void setup()
{

  pinMode(PIN_OUTP102_3, OUTPUT);
  pinMode(PIN_OUTP102_4, OUTPUT);
  pinMode(PIN_OUTP102_5, OUTPUT);
  //pinMode(PIN_BACKLIGHT, OUTPUT); // removed by Josef, no backlight anymore
  //pinMode(PIN_7LEDS, OUTPUT); // Josef: no PIN_7LEDS pin anymore
  pinMode(PIN_SERVER_POWER, OUTPUT); // power

  setupBandDimmer(); // Josef

  digitalWrite(PIN_SERVER_POWER, LOW); // do not press power


//  setBacklight(150); // removed by Josef, no backlight anymore
  keyb = new Keyb(&Wire);

  //set default logo color
  analogWrite(PIN_OUTP102_3, 200);
  analogWrite(PIN_OUTP102_4, 100);
  analogWrite(PIN_OUTP102_5, 000);
  
  Serial.begin(9600);

/*
  analogWrite(PIN_OUTP102_3, 00);
  analogWrite(PIN_OUTP102_4, 0);
  analogWrite(PIN_OUTP102_5, 000);
  */  
  
    // needed to prevent LED band from flickering
  keyb->workaroundSetCmdNormal();
  delay(500);
  keyb->workaroundSetCmdNormal();
  
  initialMode();
}

void loop()
{
  switch (mode)
  {
    case modeWelcome:
      welcomeFunc();
      break;
    case modeMainMenu:
      mainMenuFunc();
      break;
    case modeShutdown:
      shutdownFunc();
      break;
    default:
      unknownModeFunc();
      break;
  }
}

