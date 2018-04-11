/*
 * RPiOmniDrive.c
 *
 * Created: 9/27/2014 4:34:48 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>
#include "LCD_640.h"

#define true 1
#define false 0

void pwm_init(void)
{
	SETBIT(DDRE, BIT(3));
	SETBIT(DDRE, BIT(4));
	SETBIT(DDRH, BIT(3));
	SETBIT(DDRH, BIT(4));
	SETBIT(DDRH, BIT(6));
	SETBIT(DDRH, BIT(7));
	SETBIT(DDRG, BIT(4));
	SETBIT(DDRL, BIT(0));

	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
}

void Usart_init(void)
{
	DDRD &= ~(1<<PD2);
	DDRD |= (1<<PD3);
	
	PORTD &= ~(1<<PD2);
	PORTD |= (1<<PD3);
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ01);
	UCSR1A |= (1<<U2X1);
	UBRR1H = 0;
	UBRR1L = 191;
}

uint8_t UartReceive(void)
{
	PORTD |= (1<<4);
	while( !(UCSR1A & (1<<RXC1)) );
	PORTD &= ~(1<<4);
	return UDR1;
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
}

int main(void)
{
	DDRD |= (1<<4);
	PORTD &= ~(1<<4);
	Usart_init();
	pwm_init();
	//lcd_init();
	int a =0,b=0,c=0;
	int Stop[3];
	int rec;
    while(1)
    {
		rec = UartReceive();
		
		if (rec == 'A')		OCR3A = 7.37*UartReceive();
        else if(rec == 'B') OCR3B = 7.37*UartReceive();
		else if(rec == 'C')	OCR4A = 7.37*UartReceive();
		else if(rec == 'D')	a = UartReceive();
		else if(rec == 'E')	b = UartReceive();
		else if(rec == 'F')	c = UartReceive();
		if (a ==1)
		{
			SETBIT(PORTH, BIT(6));
			Stop[0] = true;
		} 
		else if(a == 2)
		{
			CLEARBIT(PORTH, BIT(6));
			Stop[0] = true;
		}
		else if (a == 3)
		{
			if (Stop[0] == true)
			{
				TOGGLEBIT(PORTH, BIT(6));						//for braking
				Stop[0] = false;
			}
		}
		if (b == 1)
		{
			SETBIT(PORTH, BIT(7));
			Stop[1] = true;
		}
		else if(b == 2)
		{
			CLEARBIT(PORTH, BIT(7));
			Stop[1] = true;
		}
		else if (b == 3)
		{
			if (Stop[1] == true)
			{
				TOGGLEBIT(PORTH, BIT(7));
				Stop[1] = false;
			}
		}
		if (c == 1)
		{
			SETBIT(PORTG, BIT(4));
			Stop[2] = true;
		}
		else if(c == 2)
		{
			CLEARBIT(PORTG, BIT(4));
			Stop[2] = true;
		}
		else if (c == 3)
		{
			if (Stop[2] == true)
			{
				TOGGLEBIT(PORTG, BIT(4));
				Stop[2] = false;
			}
		}
    }
}