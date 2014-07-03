
#include "stdint.h"
#include "def.h"
#include <stdio.h>

const int TIMER_PSC_1SEC = 10000;
const int TIMER_ARR_1SEC = 4800;
const int TIMER_ARR_DEBNC = 480;

uint32_t state;

void TIM2_IRQHandler(void)
{
  static uint16_t counter=0;

  if ((++counter & 0x0001) == 0)
    SET_LED2_OFF;
  else
    SET_LED2_ON;
  TIM2->SR &= ~(TIM_SR_UIF);
}

void TIM3_IRQHandler(void)
{
  TIM3->CR1 &= ~TIM_CR1_CEN;    // Timer 3 up-counter is disable
  TIM3->SR &= ~(TIM_SR_UIF);

  EXTI->IMR |= EXTI_IMR_MR1;
}

void EXTI1_IRQHandler(void)
{
  if (READ_BIT(GPIOB->IDR, BIT9))
    SET_LED1_ON;
  else
    SET_LED1_OFF;
  EXTI->IMR &= ~EXTI_IMR_MR1;
  EXTI->PR |= EXTI_PR_PR1;
  TIM3->CR1 |= TIM_CR1_CEN;     // Timer 3 up-counter is enable
}

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

  RCC->CFGR |= RCC_CFGR_PLLSRC;       // HSE clock selected as PLL input clock
  RCC->CFGR &= ~(RCC_CFGR_PLLXTPRE);  // no HSE divider for PLL entry
  RCC->CFGR |= RCC_CFGR_PLLMULL_2;    // PLL input clock x6 = 8MHz * 8 = 48MHz
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  RCC->CR |= RCC_CR_HSEON | RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);

  // Enable clock of timer
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM2EN;

  // Enable clock of GPIO
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);

  // Set OUTPUT
  pin = 1;
  mode = (uint32_t) 0x05 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOB->CRH, mask, mode);   // BIT9 output 10 MHz open-drain

  pin = 0;
  mode = (uint32_t) 0x05 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOB->CRH, mask, mode);   // BIT8 output 10 MHz open-drain

  // Set INPUT
  pin = 0;
  mode = (uint32_t) 0x08 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOC->CRL, mask, mode);   // BIT0 input push-pull
  SET_BIT(GPIOC->ODR, BIT0);            // Set PC0 to high for input

  pin = 1;
  mode = (uint32_t) 0x08 << (pin * 4);
  mask = (uint32_t) 0x0f << (pin * 4);
  MODIFY_REG(GPIOC->CRL, mask, mode);   // BIT1 input push-pull
  SET_BIT(GPIOC->ODR, BIT1);            // Set PC0 to high for input
  AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PC;
  EXTI->IMR |= EXTI_IMR_MR1;
  EXTI->EMR |= EXTI_EMR_MR1;
  EXTI->FTSR |= EXTI_FTSR_TR1;

  SET_LED1_OFF;
  SET_LED2_OFF;
}

void timer_init(void)
{
  TIM2->PSC = TIMER_PSC_1SEC;       // prescaler
  TIM2->ARR = TIMER_ARR_1SEC;       // autoreload
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 |= TIM_CR1_CEN;         // Timer 2 up-counter is enable

  TIM3->PSC = TIMER_PSC_1SEC;       // prescaler
  TIM3->ARR = TIMER_ARR_DEBNC;      // autoreload
  TIM3->DIER |= TIM_DIER_UIE;
}

//################## Interrupt Configuration Routine #########################
void NVIC_Configuration(void)
{
  NVIC_EN0_R |= (1<<TIM2_IRQn);     // Enable Timer 2
  NVIC_EN0_R |= (1<<TIM3_IRQn);     // Enable Timer 3
  NVIC_EN0_R |= (1<<EXTI1_IRQn);    // Enable EXTI 1
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
    while (BUTTON2 != BIT0);
    return 1;
  } else {
    return 0;
  }
}

void main(void)
{
  board_init();
  NVIC_Configuration();
  timer_init();
  state = 0;
#ifdef DEBUG
  debug_printf("hello world\n");
#endif

  while (1) {
    switch (state) {
      case 0:
        if (get_button()) {
          state = 1;
          set_leds(state);
        }
        break;
      case 1:
        if (get_button()) {
          state = 0;
          set_leds(state);
        }
        break;
    }
  }
}

