#include "uart.h"

int _write(int file, char *ptr, int len);

void usart_setup(void)
{
  rcc_periph_clock_enable(RCC_USART1);

  //// remap UART1 to port B (PB6: TX, PB7: RX)
  //AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;

  /* Enable the USART1 interrupt. */
  nvic_enable_irq(NVIC_USART1_IRQ);

  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  //gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                //GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

  /* Setup UART parameters. */
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX_RX);

  /* Enable USART1 Receive interrupt. */
  USART_CR1(USART1) |= USART_CR1_RXNEIE;

  /* Finally enable the USART. */
  usart_enable(USART1);
}

int _write(int file, char *ptr, int len)
{
  int i;

  if (file == 1) {
    for (i = 0; i < len; i++)
      usart_send_blocking(USART1, ptr[i]);
    return i;
  }

  errno = EIO;
  return -1;
}

