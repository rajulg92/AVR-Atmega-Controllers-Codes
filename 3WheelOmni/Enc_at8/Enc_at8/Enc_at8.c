/*
 * Enc_at8.c
 *
 * Created: 4/26/2014 4:06:56 PM
 *  Author: YateenKedare
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
uint8_t Direction = 0;
void Usart_init(void)
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
}

uint8_t UartReceive(void)
{
	while( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void Timer_init(void)
{
	TCCR1B = 0x07;
	DDRD &= ~(1<<PD5);
	PORTD |= (1<<PD5);
}

int main(void)
{
	sei();
	int c;
	Usart_init();
	Timer_init();
	//---------Interrupt init-------------------
	DDRD &= ~(1<<2);
	DDRD &= ~(1<<3);
	PORTD |= (1<<2) | (1<<3);
	MCUCR |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	GICR |= (1 << INT0) | (1 << INT1);
	//-----------------------------------------
	
    while(1)
    {
        c = UartReceive();
		switch(c)
		{
			case 10:
				
				UartTransmit(TCNT1L);
				UartTransmit(TCNT1H);
				
			break;
			
			case 30:
				TCNT1 = 0;
			break;
			
			case 50:
				UartTransmit(Direction);
			break;
		}
    }
}

ISR(INT0_vect)
{
	if (((bit_is_set(PIND, 2)) && (bit_is_set(PIND, 3)) ) || ( (bit_is_clear(PIND, 2)) && (bit_is_clear(PIND, 3)) ) )
	{
		Direction = 1;
	}
	else
	{
		Direction = 2;
	}
}


ISR(INT1_vect)
{
	if (((bit_is_set(PIND, 2)) && (bit_is_set(PIND, 3)) ) || ( (bit_is_clear(PIND, 2)) && (bit_is_clear(PIND, 3)) ) )
	{
		Direction = 2;
	}
	else
	{
		Direction = 1;
	}
}




/*
Table 1. Clockwise Shaft Rotation Signal Data
Time Slot (TN) Channel B Channel A
	T0 0 1
	T1 1 1
	T2 1 0
	T3 0 0
	T4 0 1
	T5 1 1
	T6 1 0
	T7 0 0
	T8 0 1
	T9 1 1
	
Table 2. Counterclockwise Shaft Rotation Signal Data
Time Slot (TN) Channel B Channel A
	T0 1 1
	T1 0 1
	T2 0 0
	T3 1 0
	T4 1 1
	T5 0 1
	T6 0 0
	T7 1 0
	T8 1 1
	T9 0 1
*/