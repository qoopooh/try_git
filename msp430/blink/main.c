#include <legacymsp430.h>
#include "def.h"
#include "flags.h"
#include "init.h"

flags flags_00;
char buff[122]; 
word speed;

/** Delay function. **/
void delay(const unsigned int d)
{
  unsigned int i;

  for (i = 0; i<d; i++) {
    nop();
  }
}

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Must stop WDT
  InitializeClock();
  InitializeLeds();
  InitializeButton();
  speed = 12000;
  eint();       // same as _BIS_SR(GIE);

  while(1) {
    delay(60000);
    LED_OUT ^= LED2;
  }

  return 0;
}

interrupt(TIMER0_A1_VECTOR) TIMERA0_ISR(void)
{
  switch (TAIV) {
    case TA0IV_TACCR1:
      TACCTL2 &= ~(CCIFG);
      break;
    case TA0IV_TACCR2:
      TACCTL1 &= ~(CCIFG);
      break;
    case TA0IV_TAIFG:
      TACTL &= ~(TAIFG);
      if (f_toggle_led)
        LED_OUT ^= LED1;
      break;
    default:
      TACTL &= ~(TAIFG);
      break;
  }
}

interrupt(PORT1_VECTOR) PORT1_ISR(void)
{
  f_toggle_led ^= 1;
  BUTTON_IFG &= ~BUTTON;
}


