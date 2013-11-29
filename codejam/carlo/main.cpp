#include <iostream>
#include <cstring>
#include "encrypt.h"

using namespace std;

unsigned char encr_decr_message[MAX_TX_BUFF_LEN];      // encrypted message

const char *unit_id = \
"11111111111111111111111111111111111111111111111111111111111111111111111111111111";
//"11111111112222222222333333333344444444441111111111222222222233333333334444444444";

const char *k_list = "E,L,1\n";

unsigned char msg[80];
char phone_id;

int main (int argc, char* argv[])
{
  phone_id = '2';
  memcpy(msg, k_list, strlen(k_list));
  cout << "UUID: " << unit_id << endl;
  cout << "Unit: " << phone_id << endl;
  cout << "Message: " << msg << endl;

  Encrypt(msg, phone_id);
  cout << "Encrypt message: " << encr_decr_message << endl;
  Decrypt(encr_decr_message);
  cout << "Decrypt message: " << encr_decr_message << endl;

  return 0;
}

