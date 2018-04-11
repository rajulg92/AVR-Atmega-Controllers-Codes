/*
 * Techfest2014.c
 *
 * Created: 12/18/2013 4:50:32 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "MOTION.h"
#define F_CPU 1000000UL
int i_right = 1;

void TLA(int x)
{
	/*if(bit_is_clear(PINA, 0) && bit_is_clear(PINA, 1) && bit_is_set(PINA, 2)) 		//front sensors off and central on and left on
	{
		fwrd(15);*/
		if(bit_is_clear(PINA, 0) && bit_is_clear(PINA, 1) && bit_is_set(PINA, 2) && bit_is_set(PINA, 3))
		{
			stop();
			_delay_ms(5000);
			/*stop();
			while((bit_is_set(PINA, 0) || bit_is_set(PINA, 1)))	//front on
			{
				left_back(x);
				right_fwrd(x);
			}
		}*/
		
		while((bit_is_clear(PINA, 0) && bit_is_clear(PINA, 1)))	//front off
		{
		left_back(x-5);
		right_fwrd(x-5);
		}	
	}
	stop();
}

void detect_stripes_left()
{
	int i_left = 0;
	if(bit_is_clear(PINA, 2) && bit_is_set(PINA, 3))
	{
		if( (bit_is_set(PINA, 0) || bit_is_set(PINA, 1)) && bit_is_set(PINA, 2) && bit_is_set(PINA, 3) )	//front on(any one) & left on
			i_left++;
	}
}

void detect_strips_right()
{
	//int i_right = 1;
	if (bit_is_set(PINA, 0) || bit_is_set(PINA, 1))
	{
		if (bit_is_set(PINA, 3) && bit_is_clear(PINA, 2))
		{
			_delay_ms(5);
			if (bit_is_set(PINA, 3) && bit_is_set(PINA, 2))
			{
				i_right++;
				
				PORTB |= 1<<i_right;
			}
		}
	}
}

void TRA()
{
	
}

void trace_line(int x)
{
	if(bit_is_clear(PINA, 0) && bit_is_clear(PINA, 1))
	{
		fwrd(x-15);
	}
	else if(bit_is_set(PINA, 0) && bit_is_set(PINA, 1))
	{
		fwrd(x-10);
	}
	else if(bit_is_clear(PINA, 0))		//left is clear
	{
		left_fwrd(x + 15);
		right_fwrd(x - 15);
	}
	else if(bit_is_clear(PINA, 1))		//right is clear
	{
		left_fwrd(x - 15);
		right_fwrd(x + 15);
	}
}

void PartA()
{
}

void main()
{
	DDRA = 0x00;
	DDRB = 0xFF;
	DDRC = 0x00;
	DDRD = 0xFF;
	
	PORTA =0x00;
	PORTB =0x00;
	PORTC =0x00;
	PORTD =0x00;
	
	TCCR1A=0xF0;
	TCCR1B=0x11;
	TCCR0 |= 1<<CS00;
	ICR1=100;
	int y = 0;
		int i_right = 1;
	while(1)
	{
	OCR1A = 50;
		/*int x = 50;
		//trace_line(40);
		//TLA(30);
		OCR1A = x;
		PORTD |= 1<<PIND6;
		OCR1B = x;
		PORTD |= 1<<PIND3;
			
			if(TCNT0 >= 200)
			{
				TCNT0 = 0;
				y++;
				//PORTB ^= 1<<PINB0;
				if(y > 15000) 
				{PORTB ^= 1<<PINB0;
					y = 0;
				}					
			}			
		
		//_delay_ms(3000);
		/*stop();
		_delay_ms(3000);*/
		detect_strips_right();
	
	}
}	
