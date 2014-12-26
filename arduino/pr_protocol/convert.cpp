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

char * Convert::AsciiToHex(char const *in_buff, int nr_words, char *out_buff)
{
  int i,j;

  for ( i = 0; i<(nr_words*2); i++ ) {
    j = i>>1;
    if ( in_buff[i] > '9' )
      out_buff[j] = in_buff[i] - 0x37;
    else
      out_buff[j] = in_buff[i] - 0x30;
    out_buff[j] <<= 4;
    i++;
    if ( in_buff[i] > '9' )
      out_buff[j] |= (in_buff[i] - 0x37);
    else
      out_buff[j] |= (in_buff[i] - 0x30);
  }

  return out_buff;
}

