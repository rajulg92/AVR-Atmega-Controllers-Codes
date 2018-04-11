#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "PORT.h"
//640

char recieve_data()
{
	while(!(UCSR2A & (1<<RXC2)))
	{}
	return UDR2;
	CLEARBIT(UCSR2A,BIT(7));
	
}

char transmit_data(char data1)
{
	_delay_ms(15);
	while(!(UCSR2A & (1<<UDRE2)))
	{}
	UDR2=data1;
}

int usart_init(void)
{

UBRR2L=12;//SET BAUD RATE TO 9600 ASYNC
UBRR2H=0;
SETBIT(UCSR2A,BIT(1));//DOUBLE ASYNC MODE ENABLED
SETBIT(UCSR2B,BIT(4));
UCSR2C=0X06;//PARITY DISABLED ASYNC 8 BIT
return 0;
}

int main (void)
{

char data[32],data1='B';
int i;

	DDRB=0XFF;	
	DDRC=0XFF;
	DDRG=0XFF;
	DDRH=0XFD;
	DDRK=0XFF;
	lcd_init();
	usart_init();
	while(1)
	{
		//if(!(UCSR2A & (1<<RXC2)))
			//{
			for(i=0;i<32;i++)
			{
			data[i]=recieve_data();
			//lcd_gotoxy1(0);
			lcd_char(data[i]);
			}
			//}
		/*else
			{
			if(data == 'A')
				{
				transmit_data(data1);
				}
			else
				{
				transmit_data('F');
				}
			}*/
	}
}
