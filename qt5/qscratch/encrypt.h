#define LINE_FEED '\n'
#define MAX_TX_BUFF_LEN 80

class Encryption {
public:
  explicit Encryption();
  void Encrypt(unsigned char *msg, char phone_id);
  void Decrypt(unsigned char *encrypted_msg);
  void setUnitId(const char *id, int len);

  unsigned char encr_decr_message[MAX_TX_BUFF_LEN + 2];      // encrypted message
  char unit_id[MAX_TX_BUFF_LEN];
};
