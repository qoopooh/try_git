#include "ccan.h"

struct can_tx_msg {
  uint32_t std_id;
  uint32_t ext_id;
  uint8_t ide;
  uint8_t rtr;
  uint8_t dlc;
  uint8_t data[8];
};

struct can_rx_msg {
  uint32_t std_id;
  uint32_t ext_id;
  uint8_t ide;
  uint8_t rtr;
  uint8_t dlc;
  uint8_t data[8];
  uint8_t fmi;
};

struct can_tx_msg can_tx_msg;
struct can_rx_msg can_rx_msg;

void can_setup(void)
{
  /* Enable peripheral clocks. */
  //rcc_periph_clock_enable(RCC_AFIO);

  //#ifdef STM32F103R
  //rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_CAN);

  /* Configure CAN pin: RX (input pull-up). */
  gpio_set_mode(GPIO_BANK_CAN1_RX, GPIO_MODE_INPUT,
      GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_RX);
  gpio_set(GPIO_BANK_CAN1_RX, GPIO_CAN1_RX);

  /* Configure CAN pin: TX. */
  gpio_set_mode(GPIO_BANK_CAN1_TX, GPIO_MODE_OUTPUT_50_MHZ,
      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_TX);
//#else
  ////rcc_periph_clock_enable(RCC_GPIOB);
  //rcc_periph_clock_enable(RCC_CAN);

  //AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;

  ///* Configure CAN pin: RX (input pull-up). */
  //gpio_set_mode(GPIO_BANK_CAN1_PB_RX, GPIO_MODE_INPUT,
      //GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_PB_RX);
  //gpio_set(GPIO_BANK_CAN1_PB_RX, GPIO_CAN1_PB_RX);

  ///* Configure CAN pin: TX. */
  //gpio_set_mode(GPIO_BANK_CAN1_PB_TX, GPIO_MODE_OUTPUT_50_MHZ,
      //GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_PB_TX);
//#endif

  /* NVIC setup. */
  nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
  nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

  //nvic_enable_irq(NVIC_CAN_RX1_IRQ);
  //nvic_set_priority(NVIC_CAN_RX1_IRQ, 1);

  /* Reset CAN. */
  can_reset(CAN1);

  /* CAN cell init. */
  if (can_init(CAN1,
      false,            /* TTCM: Time triggered comm mode? */
      true,             /* ABOM: Automatic bus-off management? */
      false,            /* AWUM: Automatic wakeup mode? */
      false,            /* NART: No automatic retransmission? */
      false,            /* RFLM: Receive FIFO locked mode? */
      false,            /* TXFP: Transmit FIFO priority? */
      CAN_BTR_SJW_1TQ,
      CAN_BTR_TS1_3TQ,
      CAN_BTR_TS2_4TQ,
      12,               /* BRP+1: Baud rate prescaler */
      false,            /* loopback mode */
      false))           /* silent mode */
  {
    /* Die because we failed to initialize. */
    while (1)
      __asm__("nop");
  }

  /* CAN filter 0 init. */
  can_filter_id_mask_32bit_init(CAN1,
      0,     /* Filter ID */
      0,     /* CAN ID */
      0,     /* CAN ID mask */
      0,     /* FIFO assignment (here: FIFO0) */
      true); /* Enable the filter. */

  /* Enable CAN RX interrupt. */
  can_enable_irq(CAN1, CAN_IER_FMPIE0);
}
