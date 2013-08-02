
#define     LED1                  BIT0
#define     LED2                  BIT6
#define     LED_DIR               P1DIR
#define     LED_OUT               P1OUT


 
#define     BUTTON                BIT3
#define     BUTTON_OUT            P1OUT
#define     BUTTON_DIR            P1DIR
#define     BUTTON_IN             P1IN
#define     BUTTON_IE             P1IE
#define     BUTTON_IES            P1IES
#define     BUTTON_IFG            P1IFG
#define     BUTTON_REN            P1REN

#define     TXD                   BIT1                      // TXD on P1.1
#define     RXD                   BIT2                      // RXD on P1.2

#define     APP_STANDBY_MODE      0
#define     APP_APPLICATION_MODE  1

#define     TIMER_PWM_MODE        0   
#define     TIMER_UART_MODE       1
#define     TIMER_PWM_PERIOD      2000  
#define     TIMER_PWM_OFFSET      20

#define     TEMP_SAME             0
#define     TEMP_HOT              1
#define     TEMP_COLD             2

#define     TEMP_THRESHOLD        5  

//   Conditions for 9600/4=2400 Baud SW UART, SMCLK = 1MHz
#define     Bitime_5              0x05*4                      // ~ 0.5 bit length + small adjustment
#define     Bitime                13*4//0x0D    
 
#define     UART_UPDATE_INTERVAL  1000

