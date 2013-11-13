#include <iostream>
#include <cstring>
#include "encrypt.h"

using namespace std;

unsigned char encr_decr_message[MAX_TX_BUFF_LEN];      // encrypted message

const char *unit_id = \
"11111111112222222222333333333344444444441111111111222222222233333333334444444444";

const char *k_list = "E,L,1\n";

char msg[80];
char phone_id;

int main (int argc, char* argv[])
{
  phone_id = '8';
  memcpy(msg, k_list, strlen(k_list));
  cout << "Decrypt beginning" << endl;
  cout << unit_id << endl;
  cout << phone_id << endl;
  cout << msg << endl;

  Encrypt(msg, phone_id);
  cout << "Decrypt ends" << endl << encr_decr_message << endl;

  return 0;
}
