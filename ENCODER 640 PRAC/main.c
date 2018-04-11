

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PORT.h"
#include "LCD640.h"

int pwm_init()
{
	DDRE=0XFF;
	TCNT3=0;
	TCCR3A=0XAA;
	TCCR3B=0X11;
	TCCR3C=0X00;
	ICR3=737;
}


int counter1_init()
{

TCCR1B=0X07;


}


int main()
{
DDRD=0X00;

counter1_init();
pwm_init();
lcd_init();
while(1)
{
lcd_gotoxy1(0);
lcd_string("TCNT1H=");
lcd_showvalue(TCNT1H);
lcd_gotoxy2(0);
lcd_string("TCNT1L=");
lcd_showvalue(TCNT1L);

if(TCNT1>=400)
{
OCR3A=0;
OCR3B=0;
CLEARBIT(PORTH,BIT(6));
CLEARBIT(PORTH,BIT(7));
}
else
{
OCR3A=300;
OCR3B=300;
SETBIT(PORTH,BIT(6));
SETBIT(PORTH,BIT(7));
}
}
}