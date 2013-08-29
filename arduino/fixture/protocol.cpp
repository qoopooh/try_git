#include "protocol.h"

const int Protocol::ReturnChar = '\r'; /* 0x0D */

Protocol::Protocol():
  completed(false),
  inputPacketIdx(0),
  commandValue(-1)
{
}

int Protocol::available()
{
  int value = 0;

  // send data only when you receive data:
  while (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();

    if (incomingByte == ReturnChar) {
      calculateResult();
      value = 1;
      break;
    }

    if (((incomingByte >= 0x30) && (incomingByte < 0x3A)) ||
          ((incomingByte >= 'A') && (incomingByte < 'G'))) {
      inputPacket[inputPacketIdx++] = static_cast<char>(incomingByte);
    }
  }  

  return value;
}

void Protocol::calculateResult()
{
  int i;
  int _iValIdx = 0;

  if (inputPacketIdx%2 != 0) {
    inputPacketIdx = 0; /* wrong packet */
    return;
  }

  iVal[_iValIdx++] = inputPacketIdx/2; /* size of packet */
  Convert::AsciiToHex(static_cast<const char *>(inputPacket),
    inputPacketIdx, &iVal[_iValIdx]);
  inputPacketIdx = 0; /* clear inputPacket buffer */
  completed = true;
}

void printCharArray(char * array, int length)
{
  int i;
  char tmp[Protocol::PacketSize];

  Serial.println("printCharArray");
  Convert::HexToAscii(array, length, tmp);
  Serial.println(length);
  Serial.println(tmp);
}
char * Protocol::read()
{
  if (!completed) {
    return NULL;
  }
  completed = false;

  return iVal;
}

int Protocol::write(const char *array, int length)
{
  int result = 0;
  char tmp[PacketSize];

  Convert::HexToAscii(array, length, tmp);
  Serial.println(tmp);

  return result;
}

