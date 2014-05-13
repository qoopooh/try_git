#include <__cross_studio_io.h>
#include "stdint.h"
#include "stm32f10x.h"
#include <stdio.h>

uint32_t state;

void delay(int count)
{
  volatile int i = 0;
  while (i++ < count);
}

void board_init(void)
{
  uint32_t temp;
  uint32_t mask;
  uint32_t mode;
  uint32_t pin;

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  pin = 4;
  mode = (uint32_t) 0x05 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  temp = GPIOA->CRL & ~mask;
  GPIOA->CRL = temp | mode;

  pin = 0;
  mode = (uint32_t) 0x04 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  temp = GPIOA->CRL & ~mask;
  GPIOA->CRL = temp | mode;
}

void set_leds(unsigned on)
{
  if (on)
    GPIOA->BSRR = (1 << 4);
  else
    GPIOA->BRR = (1 << 4);
}

int get_button(void)
{
  if (GPIOA->IDR & 0x01) {
    while ((GPIOA->IDR & 0x01) == 0x01);
    return 1;
  } else {
    return 0;
  }
}

void main(void)
{
  debug_printf("hello world\n");
//  debug_exit(0);
  board_init();
  state = 0;
  while (1) {
    switch (state) {
      case 0:
        set_leds(0);
        if (get_button()) {
          state = 1;
        }
        break;
      case 1:
        set_leds(1);
        if (get_button()) {
          state = 0;
        }
        break;
    }
  }
}
