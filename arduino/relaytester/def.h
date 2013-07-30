#ifndef __DEF_H
#define __DEF_H

typedef union {
  unsigned short all_bits;
  struct {
    unsigned int bit0:1;
    unsigned int bit1:1;
    unsigned int bit2:1;
    unsigned int bit3:1;
    unsigned int bit4:1;
    unsigned int bit5:1;
    unsigned int bit6:1;
    unsigned int bit7:1;
    unsigned int bit8:1;
    unsigned int bit9:1;
    unsigned int bit10:1;
    unsigned int bit11:1;
    unsigned int bit12:1;
    unsigned int bit13:1;
    unsigned int bit14:1;
    unsigned int bit15:1;
  } flg;
} flags;

#define TRUE                 1
#define FALSE                0


#endif /* __DEF_H */

