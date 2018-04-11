#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"LCD640.h"
//#define RES 128		//define the resolution  for the VuMeter(value from 1 to 128)


void ADCInit(void)
{
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= (1<<ADLAR) | (1<<MUX1);
	ADCSRA |= (1<<ADSC);
}

int ADCConvert(void)
{
	#define F_CPU 1000000
	ADCSRA |= (1<<ADSC);
	
	while( !(ADCSRA & (1<<ADIF)));
	return ADCH;
}

void main()
{
	int a;
	lcd_init();
	ADCInit();
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 10;
		
	DDRF &= ~(1<<2);
	PORTF |= (1<<2);
	
	DDRH = (1<<PH3);
	while(1)
	{
		#define F_CPU 14745600
		OCR4A = 5;
		a = ADCConvert() + ADCConvert() + ADCConvert() + ADCConvert() + ADCConvert();// + ADCConvert() + ADCConvert();
		a /= 5;
		lcd_gotoxy1(0);
		lcd_showvalue(a);
		lcd_string("  ");
		
		
	}
}


/*void main()
{
	lcd_init();
	DDRA = 0b11111101;
	DDRB = 0xFF;
	PORTA = 0b11111011;
	PORTB = 0xFF;
	_delay_ms(100);
	ADCSRA |= (1<<ADEN);
	ADMUX |= (1<<ADLAR);
	ADMUX |= (1<<REFS1) | (1<<REFS0);
	ADMUX |= (1<<MUX1);
	ADCSRA |= (1<<ADPS1) | (1<<ADPS2);	
	ADCSRA |= (1<<ADSC);
	
	
	while(1)
	{
		lcd_gotoxy1(0);
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		lcd_showvalue(ADCH);
		lcd_string("   ");

	}
}

*/