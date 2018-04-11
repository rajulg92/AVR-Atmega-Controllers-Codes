//ATMEGA 8 ENCODER

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "PORT.h"

int counter1_init()
{
TCCR1B=0X07;
TCNT1L=0X00;
TCNT1H=0X00;
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

int transmit_data(int data)
{
	while(!(UCSRA & (1<<UDRE)))
	{}
	UDR=data;
	CLEARBIT(UCSRA,BIT(6));
}

int main()
{
DDRB=0XFF;
DDRC=0XFF;
DDRD=0XDE;
lcd_init();
counter1_init();
usart_init();

	while(1)
	{
	lcd_gotoxy1(0);
	lcd_string("TCNT1=");
	transmit_data(TCNT1L);
	lcd_showvalue(TCNT1);
	transmit_data(TCNT1H);
	}
	
	
}