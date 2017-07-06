#ifndef UART_H

#define UART_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/common/usart_common_all.h>
#include <libopencm3/stm32/f0/usart.h>
#include <stdio.h>
#include <errno.h>

extern void usart_setup(void);
/*extern void uart1_isr(void);*/

#endif /* UART_H */
