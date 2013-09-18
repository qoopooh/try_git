#include <stdio.h>
#include "p24Fxxxx.h"

void InitIO(void);

_CONFIG1( JTAGEN_OFF & FWDTEN_OFF)
_CONFIG2( FNOSC_PRI & POSCMOD_XT )

void InitIO(void)
{
  //LATA = 0x0001;
  //LATB = 0x8004;

  AD1PCFG = 0x1FFF;

  //TRISA = 0x0000; 
  TRISB = 0x77C8;
}

int main (void)
{
  PMD1 = 0x00F9;
  PMD2 = 0x1F1E;
  PMD3 = 0x0780;
  InitIO();

  while (1) {
  }

	return 0;
}

