/*
 * Atmega640TRSKNCOE.c
 *
 * Created: 10/3/2014 12:31:23 PM
 *  Author: YateenKedare
 */ 

#include <avr/io.h>

void PWM_init(int channel)
{
    if (channel == 3)
    {
		TCCR3A = 0xA8;
		TCCR3B = 0x11;
		ICR3 = 737;
    }
	else if (channel == 4)
	{
		TCCR4A = 0xA8;
		TCCR3B = 0x11;
		ICR4 = 737;
	}
	else if (channel == 5)
	{
		TCCR5A = 0xA8;
		TCCR5B = 0x11;
		ICR5 = 737;
	}
}

