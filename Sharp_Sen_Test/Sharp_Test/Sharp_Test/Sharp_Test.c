/*
 * Sharp_Test.c
 *
 * Created: 2/28/2014 9:58:37 PM
 *  Author: YateenKedare
 */ 

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b		//Toggles the particular bit

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#define F_CPU 8000000

void AdcInit(void)
{
	ADCSRA |= (1<<ADEN) ;
	ADMUX |= (1<<REFS0);
	ADMUX |= (1<<MUX0);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1<<ADSC);
}

int main(void)
{
	lcd_init();
	DDRA = 0b11111100;
	PORTA = 0xF8;
	
    while(1)
    {
		ADCSRA |= (1<<ADSC);
		while((ADCSRA & (1<<ADSC)));
		lcd_gotoxy1(1);
		lcd_showvalue(ADC);    
    }
}