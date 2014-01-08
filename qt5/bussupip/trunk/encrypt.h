#ifndef ENCRYPT_H
#define ENCRYPT_H

#define LINE_FEED '\n'
#define MAX_TX_BUFF_LEN 80

#include <stdint.h>

typedef uint8_t byte;

extern const byte *Encrypt(const byte *msg, char phone_id);
extern const byte *Decrypt(const byte *encrypted_msg);

extern const char *unit_id;

#endif /* ENCRYPT_H */

