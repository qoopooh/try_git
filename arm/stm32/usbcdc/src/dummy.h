#ifndef __DUMMY_H

#define __DUMMY_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>

extern void take_some_break(void);
extern void clock_setup(void);
extern void systick_setup(void);
/*extern void sys_tick_handler(void);*/

#endif /* __DUMMY_H */
