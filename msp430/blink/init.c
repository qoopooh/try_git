#include "init.h"

void InitializeClock(void)
{
  byte *cal = (byte *) CALDCO_8MHZ_;

  BCSCTL1 &= (XTS + DIVA_3);
  BCSCTL1 |= (*cal & 0x0F);
  DCOCTL = *cal;
  BCSCTL3 |= LFXT1S_2;            // VLOCLK

  TACTL |= TASSEL_2 | MC_2;       // SMCLK, up to 0xffff
  TACTL |= TAIE;
  TACCR0 = 12000;
}

void InitializeLeds(void)
{
  LED_DIR |= LED1 + LED2;                          
  LED_OUT &= ~(LED1 + LED2);  
}

void InitializeButton(void)
{
  BUTTON_DIR &= ~BUTTON;
  BUTTON_OUT |= BUTTON;
  BUTTON_REN |= BUTTON;
  //BUTTON_IES |= BUTTON;
  BUTTON_IFG &= ~BUTTON;
  BUTTON_IE |= BUTTON;
}

