/*
 * Keypad16.c
 *
 * Created: 01-01-2014 18:56:01
 *  Author: Coder Rajat
 */ 


#include <avr/io.h>
#include <avr/delay.h>
//#include <avr/eeprom.h>
//#include "LCD.h"

#define F_CPU 1000000

void UartInit();
uint8_t UartReceive();
void UartTransmit(uint8_t data);

void UartInit()
{	
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
	UCSRA |= (1<<U2X);
	
	UBRRH = 0;
	UBRRL = 12;

	DDRD &= ~(1<<PD0);
	DDRD |= (1<<PD1);
	
	PORTD &= ~(1<<PD0);
	PORTD |= (1<<PD1);
	
	// PD0 - RX
	// PD1 - TX
}

uint8_t UartReceive()
{
	while( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void AdcInit()
{
	ADCSRA |= (1<<ADEN) ;
	ADMUX |= (1<<REFS0) | (1<<ADLAR);
	ADMUX |= (1<<MUX1) | (1<<MUX2);
	ADCSRA |= (1<<ADPS2) | (1<<ADSC);
}

//PC0-7 -> 4 DPDT
//PA7-PA6 -> 1 DPDT
//PD2-PD7 -> 5 PN

int main(void)
{
	uint8_t c = 0;
	DDRD = 0x00;
	PORTD = 0xFF;
	
	DDRC = 0x00;		// DPDT DATA
	PORTC = 0xFF;
	
	DDRB = 0x00;
	PORTB = 0xFF;
	
	DDRA = 0x00;		// DPDT DATA PA6-PA7
	PORTA = 0xFF;
	
	//lcd_init();
	UartInit();
	AdcInit();
	
	while(1)
	{
			c  = UartReceive();
			
			switch(c)
			{
				case 50:
					UartTransmit(PINA);
				break;
				
				case 60:
					UartTransmit(PINB);
				break;
				
				case 70:
					UartTransmit(PINC);
				break;
				
				case 80:
					UartTransmit(PIND);
				break;
				case 90:
					ADCSRA |= (1<<ADSC);
					while( !(ADCSRA & (1<<ADIF)));
					UartTransmit(ADCH);
				break;
			}
	}
}

	//////////////////////////////////////////////////////////////////////////
	//while(1)
	//{
	//ADCSRA |= (1<<ADSC);
	//while( !(ADCSRA & (1<<ADIF)));
	//lcd_gotoxy1(1);
	//lcd_showvalue(ADCH);
	//}
	//////////////////////////////////////////////////////////////////////////