#include<avr/io.h>
#include<avr/interrupt.h>
#include"PORT.h"
#include<util/delay.h>

void pwm_init()
{
	TCCR1A=0XA0;
	TCNT1=0;
    TCCR1B=0X11;
    ICR1=552;
}

void main()
{	
DDRB=0xFF;     
PORTB=0xFF;
pwm_init();
OCR1A=0;

while(1)
{
while(OCR1A<450)
{
OCR1A++;
_delay_ms(5);
}
while(OCR1A>50)
{
OCR1A--;
_delay_ms(5);
}
}
}



