/* Includes ------------------------------------------------------------------*/
#include <stdio.h> 

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "mini-printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const uint32_t k_toggle_delay = 1000;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void ledInit(void);
void uartInit(void);
void delay(uint32_t ms);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
* @brief  Main program.
* @param  None
* @retval None
*/ int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f10x_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f10x.c file
  */     

  ledInit();
  uartInit();

  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
  //printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
  snprintf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r", 24, NULL);

  while (1) {
    delay(k_toggle_delay);
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    delay(k_toggle_delay);
    GPIO_SetBits(GPIOB, GPIO_Pin_9);
  }

  return 0;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1, (uint8_t) ch);

  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void ledInit(void)
{
  /* GPIOB Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PB8 and PB9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void uartInit(void)
{
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

void delay(uint32_t ms)
{
  const uint32_t k_delay_loop = 1000;
  volatile uint32_t i, j;

  for (i=0; i<k_delay_loop; ++i) {
    for (j=0; j<k_delay_loop; ++j) {
    }
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1) {
  }
}
#endif

