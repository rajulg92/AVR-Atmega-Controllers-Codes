#include<avr/io.h>
#include<util/delay.h>
#include"PORT.h"
#include"LCD.h"

void rev()
{
	PORTD |= (1<<PD3);
	PORTD &= ~(1<<PD2);
while(OCR1A<=18)
{
	OCR1A++;
	OCR1B++;
	_delay_ms(12);
}
}

void fwd()
{
	PORTD &= ~(1<<PD3);
	PORTD |= (1<<PD2);
while(OCR1A<=25)
{
	OCR1A++;
	OCR1B++;
	_delay_ms(12);
	
}
}


void right()
{
	
	PORTD |= (1<<PD3);
	PORTD &= ~(1<<PD2);
	while(OCR1A<=20)
	{
		OCR1A++;
		OCR1B=0;
		_delay_ms(15);
	
	}
}

void left()
{
	PORTD |= (1<<PD3);
	PORTD &= ~(1<<PD2);
	while(OCR1B<=20)
	{
	OCR1A=0;
	OCR1B++;
	_delay_ms(15);
	}

//	OCR1A=0;
	//OCR1B=0;


}
void stop()
{
	OCR1A = 0;
	OCR1B = 0;

}

void main()
{	
	int x=0,y=0,z=0;
	DDRA=0x00;
	//PORTB=0xFF;
	ADCSRA=0b10001001;
	ADMUX=0b00100000;
	//DDRA |= (1<<PA3);
	PORTA |= (1<<PA3);
	DDRA |= (1<<PA4);
	PORTA &=~(1<<PA4);
	//lcd_init();
	DDRD=0xFF;
	PORTD |= (1<<PD6);
	PORTD &= ~(1<<PD7);
	TCCR1A=0xA0;
	TCCR1B=0x11;
	ICR1=50;

	//right();

	while(1)
	{
		
		ADCSRA |= (1<<ADSC);

		while(!(ADCSRA & (1<<ADIF)))
		{
		;
		}
		x=ADCH;
		//lcd_gotoxy1(0);
		//lcd_showvalue(x);
		ADMUX++;
		ADCSRA |= (1<<ADSC);

		while(!(ADCSRA & (1<<ADIF)))
		{
		;
		}
		y=ADCH;
		//lcd_gotoxy1(6);
		//lcd_showvalue(y);
		ADMUX++;
		
		ADCSRA |= (1<<ADSC);

		while(!(ADCSRA & (1<<ADIF)))
		{
		;
		}
		z=ADCH;
		//lcd_gotoxy1(12);
		//lcd_showvalue(z);
		ADMUX--;
		ADMUX--;
		if(x<60)
		{
			fwd();
		}
		else if(x>100)
		{
			rev();
		}
		else if(z<100)
			{
				if(!(PINA & (1<<PA3)))
				{
				left();
				}
				else
				{
					right();
				}
				
			}
		else
			{
			//	lcd_gotoxy1(0);
			//	lcd_string("STP");
				stop();
				_delay_ms(150);
		
			}
		
		/*
		/*else if(!(PORTA & (1<<PA3)))
		{
			lcd_gotoxy1(0);
			lcd_string("STP");
		
		}
		else
		{
			lcd_gotoxy1(0);
			lcd_string("NWS");
		}
		*/
		
	}
	
}