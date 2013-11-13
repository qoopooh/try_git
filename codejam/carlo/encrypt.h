#define LINE_FEED '\n'
#define MAX_TX_BUFF_LEN 80

void Encrypt(unsigned char *msg, char phone_id);
void Decrypt(unsigned char *encrypted_msg);

extern unsigned char encr_decr_message[MAX_TX_BUFF_LEN];      // encrypted message
extern const char *unit_id;



