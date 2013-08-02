#include <legacymsp430.h>
#include "def.h"

void InitializeLeds(void);

int main(void)
{
  unsigned char i, j;

  BCSCTL3 |= LFXT1S_2;
  TACTL |= TASSEL_1 | MC_1;
  TACCTL0 = CCIE;
  TACCR0 = 12000;
  eint();
  InitializeLeds();

  while(1) {
    for (i=0; i<100; ++i) {
      for (j=0; j<10; ++j) {
      }
    }
    LED_OUT ^= (LED1);
  }

  return 0;
}

void InitializeLeds(void)
{
  LED_DIR |= LED1 + LED2;                          
  LED_OUT &= ~(LED1 + LED2);  
}

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void)
{
  LED_OUT ^= (LED2);
}

