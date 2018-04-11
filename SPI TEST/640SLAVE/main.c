#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PORT.h"
#include "LCD.h"
//16SLAVE

char recieve_data()
{
	//_delay_ms(20);
	while(!(CHECKBIT(SPSR,BIT(7))))
	{
	;
	}
	return SPDR;
}



int spi_init(void)
{

SPCR=0X40;
return 0;
}

int main (void)
{

char data;

	DDRB=0X2F;	
	DDRA=0XFF;
	spi_init();
	lcd_init();
	while(1)
	{
	    		
		data=recieve_data();
		lcd_char(data);
		//break;
	}
	
}
