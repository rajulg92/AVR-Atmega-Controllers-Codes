#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PORT.h"
//16MASTER

char transmit_data(char data)
{	
	//_delay_ms(10);
	CLEARBIT(PORTB,BIT(4));
	SPDR=data;
	while(!(CHECKBIT(SPSR,BIT(7))))
	{
	;
	}			
    SETBIT(PORTB,BIT(4));
	
}



int spi_init(void)
{

SPCR=0X50;
return 0;
}

int main (void)
{

char data='F';

	DDRB=0XBF;	
	DDRA=0XFF;
	DDRD=0XFE;
	spi_init();
	
	while(1)
	{
	    		
			transmit_data(data);
			
	}
	
}
