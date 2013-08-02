#include <legacymsp430.h>
#include "def.h"

void InitializeLeds(void)
{
  LED_DIR |= LED1 + LED2;                          
  LED_OUT &= ~(LED1 + LED2);  
}

/** Delay function. **/
void delay(unsigned int d)
{
  int i;

  for (i = 0; i<d; i++) {
    nop();
  }
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Must have
  BCSCTL3 |= LFXT1S_2;
  TACTL |= TASSEL_1 | MC_1;
  TACCTL0 = CCIE;
  TACCR0 = 12000;
  eint();
  InitializeLeds();

  while(1) {
    LED_OUT ^= (LED2);
    delay(0x4fff);
  }

  return 0;
}

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void)
{
  LED_OUT ^= (LED1);
}

