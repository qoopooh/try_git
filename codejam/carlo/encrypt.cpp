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


//-------------------------------------
//  Encrypt routine (draft)
//    - only charaters and numbers are encrypted, symbols remain unchanged
//    - result of encrypted char is always another char
//    - routine uses a sequence of pw[] characters as password
//-------------------------------------
void Encrypt(char *msg, char phone_id)
{
  unsigned char i,shift;

  encr_decr_message[0] = phone_id;
  encr_decr_message[1] = ',';
  i=2;              // first char is phone number [1,9], not encrypted
  while (msg[i-2] != LINE_FEED)
    {
    shift = unit_id[i-1] + unit_id[0];
    encr_decr_message[i] = msg[i-2];
    if ((encr_decr_message[i] >= '0' && encr_decr_message[i] <= '9') || (encr_decr_message[i] >= 'A' && encr_decr_message[i] <= 'Z') || (encr_decr_message[i] >= 'a' && encr_decr_message[i] <= 'z'))
      {
      while (shift)
        {
        encr_decr_message[i]++;
        if (encr_decr_message[i] > '9' && encr_decr_message[i] < 'A')
          encr_decr_message[i] = 'A';
        if (encr_decr_message[i] > 'Z' && encr_decr_message[i] < 'a')
          encr_decr_message[i] = 'a';
        if (encr_decr_message[i] > 'z')
          encr_decr_message[i] = '0';
        --shift;
        }
      }
    i++;
    }
  encr_decr_message[i] = LINE_FEED;
}


//-------------------------------------
//
//-------------------------------------
void Decrypt(char *encrypted_msg)
{
  unsigned char i,shift;
  
  i=0;              // first char is phone number [1,9], not encrypted
  while (encrypted_msg[i+2] != LINE_FEED)
    {
    shift = unit_id[i+1] + unit_id[0];
    encr_decr_message[i] = encrypted_msg[i+2];
    if ((encr_decr_message[i] >= '0' && encr_decr_message[i] <= '9') || (encr_decr_message[i] >= 'A' && encr_decr_message[i] <= 'Z') || (encr_decr_message[i] >= 'a' && encr_decr_message[i] <= 'z'))
      {
      while (shift)
        {
        encr_decr_message[i]--;
        if (encr_decr_message[i] < '0')
          encr_decr_message[i] = 'z';
        if (encr_decr_message[i] > '9' && encr_decr_message[i] < 'A')
          encr_decr_message[i] = '9';
        if (encr_decr_message[i] > 'Z' && encr_decr_message[i] < 'a')
          encr_decr_message[i] = 'Z';
        --shift;
        }
      }
    i++;
    }
    encr_decr_message[i] = LINE_FEED;
    encr_decr_message[i+1] = '\0';
}




