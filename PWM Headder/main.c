#include<avr/io.h>
#include"PWM32.h"

void main(void)
{
	PWM_init(1, 737);
	
	PWM_init(2, 737);
	while(1)
	{	OCR1A = 500;
		OCR1B = 500;
		OCR2 = 200;
	}	
}
