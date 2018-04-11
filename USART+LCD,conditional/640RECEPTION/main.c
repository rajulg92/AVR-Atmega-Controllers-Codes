#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD640.h"
#include "PORT.h"
//640

char recieve_data()
{
	_delay_ms(10);
	
	while(!(UCSR2A & (1<<RXC2)))
	{}
	return UDR2;
	
	//CLEARBIT(UCSR2A,BIT(7));
	//CLEARBIT(UCSR2B,BIT(4));
	
}

char transmit_data(char data1)
{
	while(!(UCSR2A & (1<<UDRE2)))
	{}
	UDR2=data1;
	//while((CHECKBIT(UCSR2A,BIT(6))));
	//CLEARBIT(UCSR2A,BIT(6));
}

int usart_init(void)
{

SETBIT(UCSR2A,BIT(1));//DOUBLE ASYNC MODE ENABLED
SETBIT(UCSR2B,BIT(4));
SETBIT(UCSR2B,BIT(3));

UCSR2C=0X06;//PARITY DISABLED ASYNC 8 BIT

UBRR2L=191;//SET BAUD RATE TO 9600 ASYNC
UBRR2H=0;
return 0;
}

int main (void)
{

char data,data1='A';

	DDRB=0XFF;	
	DDRC=0XFF;
	DDRG=0XFF;
	DDRH=0XFD;
	DDRK=0XFF;
	lcd_init();
	usart_init();
	while(1)
	{
					
			transmit_data(data1);
			lcd_gotoxy1(0);
			lcd_char(data1);
			
		  	
	}

}