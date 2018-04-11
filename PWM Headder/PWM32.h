/*

	This header file generates PWM in specified pin
	Use the following functions
	PWM_init(num, top) -- to initialise PWM on the specific channel. Prescaling is set to 1

*/


#include<avr/io.h>
#include<util/delay.h>
#define CLK	1000000


void PWM_init(int num, int top)
{
	if(num ==1)
	{
		DDRD |= (1<<PD4) | (1<<PD5);
		PORTD &= ~(1<<PD4);
		PORTD &= ~(1<<PD5);
		TCCR1A = 0xA8;
		TCCR1B = 0x11;
		ICR1 = top;
	}
	else if(num == 2)
	{
		DDRD |= (1<<PD7);
		PORTD &= ~(1<<PD7);
		TCCR2 |= (1<<WGM20) | (1<<COM21) | (1<<CS20);
	}
	/*
	else if(num == 3)
	{
		TCCR3A = 0xA8;
		TCCR3B = 0x11;
		ICR3 = top;
	}
	else if(num ==4)
	{
		TCCR4A = 0xA8;
		TCCR4B = 0x11;
		ICR4 = top;
	}*/
}
