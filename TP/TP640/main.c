/*
 * MotorTesting.c
 *
 * Created: 3/22/2014 7:10:48 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "LCD_640.h"
#define BIT(x)  (1<<(x))
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;

void pwm_init(void)
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
	
}

int main(void)
{
	pwm_init(); 
	DDRE |= (1<<3);
    while(1)
    {
		OCR3A = 737*0.1;
		/*for(int i = 0; i <= 737; i+=20)
		{
			OCR3A = i;
			lcd_gotoxy1(0);
			lcd_showvalue((i*100)/737);
			_delay_ms(100);
		}
		for(int i=737; i>=0 ; i++)
		{
			OCR3A = i;
			lcd_gotoxy1(0);
			lcd_showvalue((i*100)/737);
			_delay_ms(100);
		}*/
	}
}