﻿#define LINE_FEED '\n'
#define MAX_TX_BUFF_LEN 80

extern void Encrypt(unsigned char *msg, char phone_id);
extern void Decrypt(unsigned char *encrypted_msg);

extern unsigned char encr_decr_message[];      // encrypted message
extern const char *unit_id;
