#ifndef CDCACM_H

#define CDCACM_H

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>


extern void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep);
extern void usb_init(void);
extern void usb_poll(void);

/*extern usbd_device *usbd_dev;*/

#endif /* CDCACM_H */
