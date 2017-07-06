
#include <string.h>
#include "cdcacm.h"
#include "uart.h"
#include "dummy.h"
#include "ccan.h"


//#define STM32F103R

#ifdef STM32F103R
#define BLINK GPIO8
#define LED GPIO9
#else
#define LED GPIO13
#endif

#define CR '\r'
#define NL '\n'

uint8_t to_hex_char(uint8_t ch);
void send_hex(uint8_t *data);
void toggle_led(void);

uint8_t to_hex_char(uint8_t ch) {
  if (ch >= 10)
    return 'A' + (ch - 10);
  return '0' + ch;
}

void send_hex(uint8_t *data) {
  uint8_t i;

  for (i = 0; data[i] != '\0'; i++) {
    uint8_t ch = data[i];

    uint8_t msb = to_hex_char((ch & 0xf0) >> 4);
    usart_send(USART1, msb);
    take_some_break();
    uint8_t lsb = to_hex_char(ch & 0x0f);
    usart_send(USART1, lsb);
    take_some_break();
  }

  usart_send(USART1, CR);
  take_some_break();
  usart_send(USART1, NL);
  take_some_break();
}

void toggle_blink(void) {
#ifdef STM32F103R
  gpio_toggle(GPIOB, BLINK);
#endif
}

void toggle_led(void) {
#ifdef STM32F103R
  gpio_toggle(GPIOB, LED);
#else
  gpio_toggle(GPIOC, LED);
#endif
}

void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep) {
  (void)ep;
  (void)usbd_dev;

  char buf[64];
  int len = usbd_ep_read_packet(usbd_dev, 0x01, buf, 64);

  if (len) {
#ifdef STM32F103R
    toggle_blink();
#else
    toggle_led();
#endif
    usbd_ep_write_packet(usbd_dev, 0x82, buf, len);
    buf[len] = 0;
    if (buf[len - 1] == CR) {
      take_some_break();
      buf[0] = NL;
      usbd_ep_write_packet(usbd_dev, 0x82, buf, 1);
    }
  }
}

void usart1_isr(void)
{
  static uint8_t data = 'A';

  /* Check if we were called because of RXNE. */
  if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

    /* Indicate that we got data. */
    //gpio_toggle(GPIOA, GPIO8);

    /* Retrieve the data from the peripheral. */
    data = usart_recv(USART1);

    /* Enable transmit interrupt so it sends back the data. */
    USART_CR1(USART1) |= USART_CR1_TXEIE;
  }

  /* Check if we were called because of TXE. */
  if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_TXE) != 0)) {

    /* Indicate that we are sending out data. */
#ifdef STM32F103R
    toggle_blink();
#else
    toggle_led();
#endif

    /* Put data into the transmit register. */

    if (data == CR) {
      usart_send(USART1, CR);
      take_some_break();
      usart_send(USART1, NL);
    } else {
      if (data >= 'a' && data <= 'z')
        data -= 0x20;
      else if (data >= 'A' && data <= 'Z')
        data += 0x20;
      usart_send(USART1, data);
    }

    /* Disable the TXE interrupt as we don't need it anymore. */
    USART_CR1(USART1) &= ~USART_CR1_TXEIE;
  }
}

//void can_rx1_isr(void) {
void usb_lp_can_rx0_isr(void) {
  uint32_t id, fmi;
  bool ext, rtr;
  uint8_t length, data[9];

  memset(data, 0, 9);

  can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &length, data);

  //if (length != 2)
    send_hex(data);

  can_fifo_release(CAN1, 0);
}

void sys_tick_handler(void)
{
  static int temp32 = 0;
  static uint8_t data[8] = {0, 1, 2, 0, 0, 0, 0, 0};

  /* We call this handler every 1ms so 100ms = 1s
  * Resulting in 100Hz message frequency.
  */
  if (++temp32 != 1000)
    return;

  temp32 = 0;

  toggle_led();

  /* Transmit CAN frame. */
  data[0]++;
  send_hex(data);

  if (can_transmit(CAN1,
        0,     /* (EX/ST)ID: CAN ID */
        false, /* IDE: CAN ID extended? */
        false, /* RTR: Request transmit? */
        8,     /* DLC: Data length */
        data) == -1) {
    send_hex("DD");
  }
}

int main(void)
{
  int i;

  clock_setup();

#ifdef STM32F103R
  gpio_set(GPIOB, LED);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
          GPIO_CNF_OUTPUT_PUSHPULL, LED);

  gpio_set(GPIOB, BLINK); // Turn off
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
          GPIO_CNF_OUTPUT_PUSHPULL, BLINK);
#else
  gpio_set(GPIOC, LED);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
          GPIO_CNF_OUTPUT_PUSHPULL, LED);
#endif

  systick_setup();

  can_setup();
  //usb_init();
  usart_setup();

  for (i = 0; i < 0x800000; i++)
    __asm__("nop");

#ifdef STM32F103R
  gpio_clear(GPIOB, LED);
#else
  gpio_clear(GPIOC, LED);
#endif

  while (1) {
    //usb_poll();

    //for (i = 0; i < 0x800000; i++)
      //__asm__("nop");
    //toggle_led();
  }
}

