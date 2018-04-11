/*
 * LED2.cpp
 *
 * Created: 12/4/2014 7:11:06 PM
 *  Author: Yateen Kedare
 */ 


#define F_CPU 1000000
#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	DDRB = 0x00;
	DDRD = 0xFF;
	PORTB =0xFF;
	PORTD =0x00;
	while(1)
	{
		if(bit_is_clear(PINB, 0))
		{
			PORTD = 0xFF;
		}
		else
		{
			PORTD = 0x00;
		}
		
	}
}