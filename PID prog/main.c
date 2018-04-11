#include<avr/io.h>
#include<util/delay.h>
#define F_CPU = 14745600
#define _clb(x,b) x&=~b
#define _sb(x,b) x|=b
#define _tb(x,b) x^=b
#define bit(x) (1 << (x))
#include "PWM640.h"
int main(void)
{
	_sb(DDRE,bit(3));
	_sb(DDRH,bit(6));
	_sb(PORTH, bit(6));
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
	while(1)
	{
		OCR3A = 700;
		_tb(PORTH, bit(6));
		_delay_ms(5000);
	}

}	