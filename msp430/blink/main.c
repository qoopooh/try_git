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
    LED_OUT |= LED2;
    delay(3000);
    LED_OUT &= ~LED2;
  }

  return 0;
}

#ifdef MSP430G2452
interrupt(TIMER0_A1_VECTOR) TIMERA0_ISR(void)
#else
interrupt(TIMERA1_VECTOR) TIMERA0_ISR(void)
#endif
{
  switch (TAIV) {
#ifdef MSP430G2452
    case TA0IV_TAIFG:
#else
    case TAIV_TAIFG:
#endif
      TACTL &= ~(TAIFG);
      if (f_toggle_led)
        LED_OUT ^= LED1;
      else
        LED_OUT &= ~LED1;
      break;
    default:
      TACTL &= ~(TAIFG);
      break;
  }
}

interrupt(PORT1_VECTOR) PORT1_ISR(void) {
  f_toggle_led ^= 1;
  BUTTON_IFG &= ~BUTTON;
}

