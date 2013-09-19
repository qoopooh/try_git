#include "p24FJ256GB106.h"

/** OUTPUT **/
#define RDR_D1_GRN    LATBbits.LATB10
#define RS422_RXD     LATBbits.LATB15
#define STATUS        LATCbits.LATC14
#define TAMPER_IN     LATDbits.LATD0
#define RDR_D0_GRN    LATDbits.LATD5
#define RS422_TX_GRN  LATDbits.LATD6
#define RS422_RX_RED  LATDbits.LATD7
#define DPS_2         LATDbits.LATD9
#define REX_1         LATEbits.LATE6
#define USB_RED       LATEbits.LATE7
#define RLY_0_CTL     LATFbits.LATF0
#define RLY_1_CTL     LATFbits.LATF1
#define REX_2         LATGbits.LATG6
#define DPS_1         LATGbits.LATG7

/** INPUT **/
#define Addr_0        LATBbits.LATB10
#define Addr_1        LATBbits.LATB10
#define Addr_2        LATBbits.LATB10
#define Addr_3        LATBbits.LATB10
#define AUX_MODE      LATGbits.LATG6
#define ACCESS_MODE   LATGbits.LATG7
#define RELAY_MODE    LATGbits.LATG7

void InitIO(void)
{
  TRISB  =  0x7BFF;
  TRISC   =  0xBFFF;
  TRISD   =  0xFD1E;
  TRISE   =  0xFF3F;
  TRISF   =  0xFFFC;  // RF0-1 are relays
  TRISG   =  0xFF3F;

  RDR_D1_GRN = 1;
  RS422_RXD = 1;
  STATUS = 1;
  TAMPER_IN = 1;
  RDR_D0_GRN = 1;
  RS422_TX_GRN = 1;
  RS422_RX_RED = 1;
  DPS_2 = 1;
  REX_1 = 1;
  USB_RED = 1;
  RLY_0_CTL = 1;
  RLY_1_CTL = 1;
  REX_2 = 1;
  DPS_1 = 1;
}

int main (void)
{
  OSCCON = 0x2200;
  InitIO();

  while (1) {
  }

	return 0;
}

