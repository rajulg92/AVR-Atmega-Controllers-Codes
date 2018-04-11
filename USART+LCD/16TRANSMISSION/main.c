#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "PORT.h"
//16

/*#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;
*/

char transmit_data(char data)
{
	_delay_ms(5);
	
	while(!(UCSRA & (1<<UDRE)))
	{}
	UDR=data;
	
}


char recieve_data()
{
	while(!(UCSRA & (1<<RXC)))
	{}
	return UDR;
	CLEARBIT(UCSRA,BIT(7));
}

int usart_init(void)
{

UBRRL=12;//SET BAUD RATE TO 9600 ASYNC
UBRRH=0;
SETBIT(UCSRA,BIT(1));//DOUBLE ASYN MODE ENABLED
SETBIT(UCSRB,BIT(3));//TRANS ENABLED
SETBIT(UCSRB,BIT(4));//RECEP ENABLED
UCSRC=0X06;//PARITY DISABLED ASYNC 8 BIT
return 0;
}

int main (void)
{

char data='B',data1;

	DDRB=0XFF;	
	DDRC=0XFF;
	DDRD=0XFE;
	
	lcd_init();
	usart_init();
	
	while(1)
	{
	    if(!(CHECKBIT(UCSRA,BIT(6))))
		{
			transmit_data(data);
			CLEARBIT(UCSRA,BIT(6));
		}
		else
		{
		data1=recieve_data();
		lcd_gotoxy1(0);
		lcd_char(data1);
		}
			
		
	}
	
}
