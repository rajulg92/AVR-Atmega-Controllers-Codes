//PORT E BITS 3,4,5


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PORT.h"

int main (void)
{
	int i;
	DDRH=0XFF;
	DDRE=0XFF;
	TCNT3=0;
	TCCR3A=0XAA;
	TCCR3B=0X15;
	TCCR3C=0X00;
	ICR3=737;
	TCCR4A=0XAA;
	TCCR4B=0X15;
	TCCR4C=0X00;
	ICR4=737;
	
	while(1)
	{
		/*
		for(i=0;i<700;i++)
		{
		OCR3A=i;
		SETBIT(PORTH,BIT(6));
		_delay_ms(10);
		}
		
		for(i=0;i<700;i++)
		{0
		OCR3A=i;
		CLEARBIT(PORTH,BIT(6));
		_delay_ms(10);
		}
		*/
		OCR3A=737*0.5;
		OCR3B=737*0.4;
		OCR3C=737*0.15;
		OCR4A=737*0.8;
		OCR4B=737*0.75;
		OCR4C=737*0.6;
	}	
	// Insert application code here, after the board has been initialized.
}
