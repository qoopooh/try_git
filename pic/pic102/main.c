#include "p24FJ256GB106.h"

void InitIO(void);

_CONFIG1( JTAGEN_OFF & FWDTEN_OFF)
_CONFIG2( FNOSC_PRI & POSCMOD_XT )

void InitIO(void)
{
  LATB = 0x8004;

  AD1PCFG = 0x1FFF;

  AD1PCFGL  =  0xFFFF;  //set to all digital I/O
  TRISB  =  0xFFFF;  //configure all PortB as input
  TRISC   =  0xFFFF;  //configure all PortC as input,

  RPINR18bits.U1RXR   =  2;   //UART1 receive set to RB2/RP2
  RPOR1bits.RP3R   =  3;   //UART1 transmit set to RB3/RP3
}

int main (void)
{
  OSCCON = 0x11C0;  //select INTERNAL RC, Post Scale PPL
  //PMD1 = 0x00F9;
  //PMD2 = 0x1F1E;
  //PMD3 = 0x0780;
  InitIO();

  while (1) {
  }

	return 0;
}

