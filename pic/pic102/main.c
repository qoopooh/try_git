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
#define L_ACCESS_MODE   LATBbits.LATB3
#define L_RELAY_MODE    LATCbits.LATC13
#define L_AUX_MODE      LATEbits.LATE5
#define L_Addr_0        LATDbits.LATD1
#define L_Addr_1        LATDbits.LATD2
#define L_Addr_2        LATDbits.LATD3
#define L_Addr_3        LATDbits.LATD4

#define P_AUX_MODE      CNPU4bits.CN63PUE

#define ACCESS_MODE   PORTBbits.RB3
#define RELAY_MODE    PORTCbits.RC13
#define AUX_MODE      PORTEbits.RE5
#define Addr_0        PORTDbits.RD1
#define Addr_1        PORTDbits.RD2
#define Addr_2        PORTDbits.RD3
#define Addr_3        PORTDbits.RD4

void InitIO(void)
{
  TRISB = 0x7BFF;
  TRISC = 0xBFFF;
  TRISD = 0xFD1E;
  TRISE = 0xFF3F;
  TRISF = 0xFFFC;  // RF0-1 are relays
  TRISG = 0xFF3F;

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

  P_AUX_MODE = 1;
}

int main (void)
{
  int x;

  OSCCON = 0x2200;
  InitIO();

  while (1) {
    RDR_D1_GRN = AUX_MODE;
    RS422_RXD = AUX_MODE;
    STATUS = AUX_MODE;
    TAMPER_IN = AUX_MODE;
    RDR_D0_GRN = AUX_MODE;
    RS422_TX_GRN = AUX_MODE;
    RS422_RX_RED = AUX_MODE;
    DPS_2 = AUX_MODE;
    REX_1 = AUX_MODE;
    USB_RED = AUX_MODE;
    RLY_0_CTL = AUX_MODE;
    RLY_1_CTL = AUX_MODE;
    REX_2 = AUX_MODE;
    DPS_1 = AUX_MODE;
  }

	return 0;
}

