#include <iostream>
#include <cstring>
#include "def.h"
#include "encrypt.h"

using namespace std;

const char *unit_id = \
  //"11111111111111111111111111111111111111111111111111111111111111111111111111111111";
  "11111111112222222222333333333344444444441111111111222222222233333333334444444444";

const char *k_list = "E,L,1\r\n";
const char *ans;

byte msg[80];
char phone_id;

int main (int argc, char* argv[])
{
  cout << argc << argv << endl;
  phone_id = '2';
  memcpy(msg, k_list, strlen(k_list));
  cout << "UUID: " << unit_id << endl;
  cout << "Unit: " << phone_id << endl;
  cout << "Message: " << msg << endl;

  ans = (const char *)Encrypt(msg, phone_id);
  cout << "Encrypt message: " << ans << endl;
  ans = (const char *)Decrypt((const byte *) ans);
  cout << "Decrypt message: " << ans << endl;

  return 0;
}

