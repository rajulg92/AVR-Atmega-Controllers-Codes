#include<avr/io.h>
#include<avr/interrupt.h>
#include"PORT.h"
#include<util/delay.h>

void pwm_init()
{
	TCCR3A=0XA8;
	TCNT3=0;
    TCCR3B=0X11;
    ICR3=737;
}

void main()
{	
DDRE=0xFF;     
//PORTB=0xFF;
pwm_init();
OCR3A=720;

while(1)
{
while(OCR3A<710)
{
OCR3A++;
_delay_ms(1);
}
while(OCR3A>50)
{
OCR3A--;
_delay_ms(1);
}
}
}



