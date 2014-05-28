#include <__cross_studio_io.h>
#include "stdint.h"
#include "def.h"
#include <stdio.h>

uint32_t state;

void delay(int count)
{
  volatile int i = 0;
  while (i++ < count);
}

void board_init(void)
{
  uint32_t mask;
  uint32_t mode;
  uint32_t pin;

  RCC->CFGR |= RCC_CFGR_PLLMULL_0 | RCC_CFGR_PLLSRC; // HSE oscillator clock selected as PLL input clock
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  RCC->CR |= RCC_CR_HSEON | RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

  // Set OUTPUT
  pin = 1;
  mode = (uint32_t) 0x05 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOB->CRH, mask, mode); // BIT9 output 10 MHz open-drain
  pin = 0;
  mode = (uint32_t) 0x05 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOB->CRH, mask, mode); // BIT8 output 10 MHz open-drain

  // Set INPUT
  pin = 0;
  mode = (uint32_t) 0x08 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOC->CRL, mask, mode); // BIT0 input push-pull
  SET_BIT(GPIOC->ODR, BIT0); // Set PC0 to high for input
  pin = 1;
  mode = (uint32_t) 0x08 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOC->CRL, mask, mode); // BIT1 input push-pull
  SET_BIT(GPIOC->ODR, BIT1); // Set PC0 to high for input

  SET_LED1_OFF;
  SET_LED2_OFF;
}

void timer_init(void)
{
  TIM2->PSC = 10; // prescaler
  TIM2->ARR = 10; // autoreload
  TIM2->DIER |= TIM_DIER_UIE;

  TIM2->CR1 |= TIM_CR1_CEN; // Timer 2 upcounter is enable
}

void set_leds(unsigned on)
{
  if (on)
    SET_LED1_ON;
  else
    SET_LED1_OFF;
}

int get_button(void)
{
  if (BUTTON2 != BIT0) {
    SET_LED2_ON;
    while (BUTTON2 != BIT0);
    SET_LED2_OFF;
    return 1;
  } else {
    return 0;
  }
}

void TIM2_IRQHandler(void)
{
  static uint16_t counter=0;

  if ((++counter & 0x0001) == 0)
    SET_LED2_OFF;
  else
    SET_LED2_ON;
  TIM2->SR &= ~(TIM_SR_UIF);
}


//################## Interrupt Configuration Routine #########################
void NVIC_Configuration(void)
{
  //NVIC_InitTypeDef NVIC_InitStructure;

  ////enable tim2 irq
  //NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
  NVIC_EN0_R |= (1<<TIM2_IRQn); // Enable Timer 2
}

void main(void)
{
//  debug_exit(0);
  board_init();
  /* Interrupt Config */
  NVIC_Configuration();
  timer_init();
  state = 0;
  debug_printf("hello world\n");

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
