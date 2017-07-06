#ifndef __CCAN_H

#define __CCAN_H

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

extern void can_setup(void);
/*extern void usb_lp_can_rx0_isr(void);*/

#endif /* __CCAN_H */
