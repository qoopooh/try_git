#include "convert.h"

char * Convert::HexToAscii(const char *in_buff, int nr_bytes, char *out_buff)
{
  int i,j;

  for (i = 0;i < nr_bytes;i++) {
    j = i<<1;
    out_buff[j] = (in_buff[i]&0xF0)>>4;
    if ( out_buff[j] > 9 )
      out_buff[j] += 0x37;
    else
      out_buff[j] += 0x30; // 0x41 - 0x0A

    out_buff[j+1] = in_buff[i] & 0x0F;
    if ( out_buff[j+1] > 9 )
      out_buff[j+1] += 0x37;
    else
      out_buff[j+1] += 0x30; // 0x41 - 0x0A
  }

  out_buff[j+2] = '\0';

  return out_buff;
}

char * Convert::AsciiToHex(char const *in_buff, int nr_hex, char *out_buff)
{
  int nr_words = nr_hex << 1;
  int j = 0;
  int i;
  char ch;

  for ( i = 0; i<nr_words; i++ ) {
    ch = in_buff[i];
    if (ch > 0x60)  // a, b, c, ...
      ch -= 0x20;   // now is A, B, C, ...
    if ( ch > '9' )
      out_buff[j] = ch - 0x37;
    else
      out_buff[j] = ch - 0x30;
    out_buff[j] <<= 4;

    ch = in_buff[++i];
    if (ch > 0x60)  // a, b, c, ...
      ch -= 0x20;   // now is A, B, C, ...
    if ( ch > '9' )
      out_buff[j] |= (ch - 0x37);
    else
      out_buff[j] |= (ch - 0x30);

    ++j;
  }

  return out_buff;
}

