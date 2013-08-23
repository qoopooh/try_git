/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

#include "LPC17xx.h"

volatile uint32_t temp;

void _delay(uint32_t del);

int main (void) 
{
	LPC_SC->PCONP |= ( 1 << 15 ); // power up GPIO
	LPC_GPIO2->FIODIR |= 1 << 2; // puts P1.29 into output mode.
	while(1)
	{
		LPC_GPIO2->FIOPIN |= 1 << 2; // make P1.29 high
		_delay( 1 << 24 );
		LPC_GPIO2->FIOPIN &= ~( 1 << 2 ); // make P1.29 low
		_delay( 1 << 24 );
	}
	return 0;
  
}
void _delay(uint32_t del)
{
	uint32_t i;
	for(i=0;i<del;i++)
		temp = i;
}
