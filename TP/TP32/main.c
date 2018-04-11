/*
 * GccApplication1.c
 *
 * Created: 16-10-2014 14:26:48
 *  Author: Sach
 */ 


#include <avr/io.h>
#include <util/delay.h>


int main(void)
{

	DDRD = 0xFF;
	PORTD = 0x00;
	while(1)
	{
		PORTD |= (1<<6);
		PORTD &= ~(1<<1);
		PORTD |= (1<<3);
		PORTD &= ~(1<<2);
	}
}
