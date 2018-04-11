#include<avr/io.h>
#include<util/delay.h>
#include "PORT.h"

void pwm_init()
{
	TCCR1A=0XA0;
	TCNT1=0;
    TCCR1B=0X11;
    ICR1=5000;
	OCR1A=500;
	OCR1B=250;
}
/*
void left()
{
OCR1A=250;
}

void right()
{
OCR1A=500;
}*/

void main()
{	
//int f=0;
DDRB=0xFF;     
PORTB=0xFF;
pwm_init();

/*
while(1)
{
if(CHECKBIT(PORTD,BIT(0)))
{
	if(!CHECKBIT(PORTD,BIT(1)))
	{
		left();
	}
}
else
if(CHECKBIT(PORTD,BIT(1)))
{
	if(!CHECKBIT(PORTD,BIT(0)))
	{
		right();
	}
}
/*while(OCR1A<490)
{
OCR1A++;
OCR1B++;
_delay_ms(20);
}
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
while(OCR1A>30)
{
OCR1A--;
OCR1B--;
_delay_ms(20);
}
_delay_ms(1000);
_delay_ms(1000);
_delay_ms(1000);
f=2;
if(f==2)
{
TOGGLEBIT(PORTB,BIT(3));
f=0;
}*/

while(1);
}