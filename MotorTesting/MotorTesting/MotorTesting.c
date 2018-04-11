/*
 * MotorTesting.c
 *
 * Created: 3/22/2014 7:10:48 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>
#include <util/delay.h>
#define BIT(x)  (1<<(x))
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;

void pwm_init(void)
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
}

int main(void)
{
	pwm_init();
	SETBIT(PORTG, BIT(4));
	SETBIT(PORTH, BIT(3));
    while(1)
    {
		TOGGLEBIT(PORTG, BIT(4));
        OCR4A = 737;
		_delay_ms(2000);
		OCR4A = 400;
		_delay_ms(2000);
		
    }
}