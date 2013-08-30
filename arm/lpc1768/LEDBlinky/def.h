
#define SET_LED1_ON             (LPC_GPIO2->FIOPIN |= 1 << 0)
#define SET_LED1_OFF            (LPC_GPIO2->FIOPIN &= ~(1 << 0))

