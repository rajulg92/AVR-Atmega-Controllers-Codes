/*
 * ServoHand.c
 *
 * Created: 9/19/2013 6:34:57 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>
#include <util/delay.h>

void topservo(void)
{
	if(bit_is_clear(PINC,2))
	{
		if(OCR1A < 1100)		
		{
			OCR1A = OCR1A++;
			_delay_us(500);
		}
	}
	else if(bit_is_clear(PINC,3)) 
	{
		if(OCR1A > 250)
		{
			OCR1A = OCR1A--;
			_delay_us(500);
		}
	}
	else
	{
		OCR1A = OCR1A;
	}
}

void bottomservo(void)	
{
	if(bit_is_clear(PINC,0))
	{	
		/*if((OCR1B > 300) )		
		{	
			OCR1B = OCR1B--;
			_delay_us(500);
		}*/OCR1B = 690;
	}
	else if(bit_is_clear(PINC,1))
	{
		/*if(OCR1B < 1150)	
		{
			OCR1B = OCR1B++;
			_delay_us(500);
		}*/OCR1B = 9900;
	}
	else
	{
		OCR1B = 0;
	}
}

int main(void)
{
	DDRB = 0b00000110;
	PORTB = 0b00000000;
	
	DDRC = 0b00000000;
	PORTC = 0b00111111;
	
	TCCR1A = 0b10100000;
	TCCR1B = 0b00010001;
	
	
	ICR1 =  10000;
	OCR1A = 620;
	OCR1B = 0;
	

	//OCR1B = 1150;
	//OCR1B = 300;
	//OCR1A = 1100;
	//OCR1A = 250;
	
    while(1)
    {
		bottomservo();
		topservo();
		//OCR1B = 9400;
	
    }
}