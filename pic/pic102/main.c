#include "p24FJ256GB106.h"

void InitIO(void)
{
  LATB = 0x8004;

  //AD1PCFG = 0xFFFF; //set to all digital I/O

  TRISB  =  0xFBFF;  //configure all PortB as input
  TRISC   =  0xFFFF;  //configure all PortC as input,
  TRISD   =  0xFF00;
  TRISF   =  0xFFFC;  // RF0-1 are relays

  LATBbits.LATB10 = 1;
  LATDbits.LATD5 = 1;
  LATF = 0x0003;
}

int main (void)
{
  OSCCON = 0x2200;
  InitIO();

  while (1) {
  }

	return 0;
}

