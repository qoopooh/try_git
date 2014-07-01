#ifndef __DEF_H
#define __DEF_H

#ifndef STARTUP_FROM_RESET
#define DEBUG
#include <__cross_studio_io.h>
#endif

#include "stm32f10x.h"
/*#include "cmsis_nvic.h"*/

#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))

#define BIT0    0x0001 
#define BIT1    0x0002 
#define BIT2    0x0004 
#define BIT3    0x0008 
#define BIT4    0x0010 
#define BIT5    0x0020 
#define BIT6    0x0040 
#define BIT7    0x0080 
#define BIT8    0x0100 
#define BIT9    0x0200 
#define BITA    0x0400 
#define BITB    0x0800 
#define BITC    0x1000 
#define BITD    0x2000 
#define BITE    0x4000 
#define BITF    0x8000 

#define BUTTON2       READ_BIT(GPIOC->IDR, BIT0)
#define BUTTON3       READ_BIT(GPIOC->IDR, BIT1)
#define SET_LED1_ON   SET_BIT(GPIOB->BRR, BIT9)
#define SET_LED1_OFF  SET_BIT(GPIOB->BSRR, BIT9)
#define SET_LED2_ON   SET_BIT(GPIOB->BRR, BIT8)
#define SET_LED2_OFF  SET_BIT(GPIOB->BSRR, BIT8)

#endif /* __DEF_H */

