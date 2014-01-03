//-----------------------------------------------------------------------------
//   Advanced Domotics                        Copyright 2012
//-----------------------------------------------------------------------------
//
//  Project:        FTC2E
//
//  Processor:      Texas Instruments MSP430F247
//  Clock:          16 MHz
//
//-----------------------------------------------------------------------------
//  Module:         encrypt.c
//
//  Contents:       encryption and decryption routines
//
//-----------------------------------------------------------------------------
#include "encrypt.h"

// GLOBAL VARIABLES
byte encr_message[MAX_TX_BUFF_LEN + 2];      // encrypted message
byte decr_message[MAX_TX_BUFF_LEN + 2];      // decrypted message


//-------------------------------------
//  Encrypt routine (draft)
//    - only charaters and numbers are encrypted, symbols remain unchanged
//    - result of encrypted char is always another char
//    - routine uses a sequence of pw[] characters as password
//-------------------------------------
const byte *Encrypt(const byte *msg, char phone_id)
{
  byte i,shift;

  encr_message[0] = phone_id;
  encr_message[1] = ',';
  i=2;              // first char is phone number [1,9], not encrypted
  while ((msg[i-2] != LINE_FEED) && (i < MAX_TX_BUFF_LEN))
    {
    shift = unit_id[i-1] + unit_id[0] - 0x60;
    encr_message[i] = msg[i-2];
    if ((encr_message[i] >= '0' && encr_message[i] <= '9')
        || (encr_message[i] >= 'A' && encr_message[i] <= 'Z')
        || (encr_message[i] >= 'a' && encr_message[i] <= 'z'))
      {
      while (shift)
        {
        encr_message[i]++;
        if (encr_message[i] > '9' && encr_message[i] < 'A')
          encr_message[i] = 'A';
        if (encr_message[i] > 'Z' && encr_message[i] < 'a')
          encr_message[i] = 'a';
        if (encr_message[i] > 'z')
          encr_message[i] = '0';
        --shift;
        }
      }
    i++;
    }
  encr_message[i] = LINE_FEED;
  encr_message[i+1] = '\0';

  return (const byte *) &encr_message;
}


//-------------------------------------
//
//-------------------------------------
const byte *Decrypt(const byte *encrypted_msg)
{
  byte i,shift;
  
  i=0;              // first char is phone number [1,9], not encrypted
  while ((encrypted_msg[i+2] != LINE_FEED) && (i < MAX_TX_BUFF_LEN))
    {
    shift = unit_id[i+1] + unit_id[0] - 0x60;
    decr_message[i] = encrypted_msg[i+2];
    if ((decr_message[i] >= '0' && decr_message[i] <= '9')
        || (decr_message[i] >= 'A' && decr_message[i] <= 'Z')
        || (decr_message[i] >= 'a' && decr_message[i] <= 'z'))
      {
      while (shift)
        {
        decr_message[i]--;
        if (decr_message[i] < '0')
          decr_message[i] = 'z';
        if (decr_message[i] > '9' && decr_message[i] < 'A')
          decr_message[i] = '9';
        if (decr_message[i] > 'Z' && decr_message[i] < 'a')
          decr_message[i] = 'Z';
        --shift;
        }
      }
    i++;
    }
  decr_message[i] = LINE_FEED;
  decr_message[i+1] = '\0';

  return (const byte *) &decr_message;
}


