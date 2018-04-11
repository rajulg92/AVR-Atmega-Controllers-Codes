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
	while(!(UCSRA & (1<<UDRE)))
	{}
	UDR=data;
}


char recieve_data()
{
	_delay_ms(20);
	while(!(UCSRA & (1<<RXC)))
	{}
	return UDR;
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

char data[32]="IMYNAMEISRAJULGUPTAIMINTHIRDYEAR",data1;
int i;

	DDRB=0XFF;	
	DDRC=0XFF;
	DDRD=0XFE;
	
	lcd_init();
	usart_init();
	
	while(1)
	{
	//if(!(UCSRA & (1<<TXC)))
	//	{
	for(i=0;i<32;i++)
		{
		transmit_data(data[i]);
		while(!(UCSRA & (1<<TXC)))
		{}
		}
		//}
	/*else
		{
		data1=recieve_data();
		lcd_gotoxy1(0);
		lcd_char(data1);
		}*/
	}
	
}
