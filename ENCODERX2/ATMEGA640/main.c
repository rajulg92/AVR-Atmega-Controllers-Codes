//ATMEGA 640 ENCODER+MOTOR

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD640.h"
#include "PORT.h"


int counter1_init()
{

TCCR1B=0X07;

}
int pwm_init()
{
	DDRE=0XFF;
	TCNT3=0;
	TCCR3A=0XAA;
	TCCR3B=0X15;
	TCCR3C=0X00;
	ICR3=737;
}

int usart_init(void)
{

UBRR1L=12;//SET BAUD RATE TO 9600 ASYNC
UBRR1H=0;
SETBIT(UCSR1A,BIT(1));//DOUBLE ASYN MODE ENABLED
//SETBIT(UCSRB,BIT(3));//TRANS ENABLED
SETBIT(UCSR1B,BIT(4));//RECEP ENABLED
UCSR1C=0X06;//PARITY DISABLED ASYNC 8 BIT
return 0;
}

int recieve_data(int data)
{
	while(!(UCSR1A & (1<<RXC1)))
	{}
	return UDR1;
}

int main()
{
int data;
DDRB=0XFF;
DDRD=0XDE;
lcd_init();
counter1_init();
usart_init();
pwm_init();
lcd_gotoxy1(0);
lcd_string("Count=");
lcd_gotoxy2(0);
lcd_string("TCNT1=");
	while(1)
	{
	OCR3A=400;
	OCR3B=400;
	lcd_gotoxy1(6);
	data=recieve_data(data);
	lcd_showvalue(data);
	lcd_gotoxy2(6);
	lcd_showvalue(TCNT1);
	}
}