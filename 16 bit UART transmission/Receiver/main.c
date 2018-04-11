/*
	AT640
*/

#include<avr/io.h>
#include<util/delay.h>
#include<interrupt.h>
#include"LCD.h"
//#include "LCD640.h"
unsigned int enc_r, a, b;

void receive_encr()
{
	for(int i = 0; i < 2; i++)
	{
		while (!(UCSRA & (1<<RXC)));
		a = UCSRB;
		b = UDR;
		
		a = a & 0x02;
		lcd_gotoxy1(0);
		lcd_showvalue(a);
		if(a == 2)
		{
			enc_r |= (b << 8);
			
		}
		else if(a == 0)
		{
			enc_r |= b;
			
		}
	}
}

void main()
{
	lcd_init();
	//lcd_showvalue(30000);
	
	UCSRB = 0xFC;
	UCSRC |=  (1 << UCSZ1)| (1<<UCSZ0);
	UBRRL=25;
	UBRRH=0;
	DDRD |= (1<<1);
	DDRD &= ~(1<<0);
	int a, b;
	while(1)
	{
	//lcd_cmd(1);
	receive_encr();
	lcd_gotoxy2(0);
		lcd_showvalue(enc_r);
		/*lcd_gotoxy1(0);
		while (!(UCSRA & (1<<RXC)));
		a = UCSRB;
		b = UDR;
		if(a == 254)
		{
			enc |= (b << 8);
			//lcd_gotoxy1(0);
			//lcd_showvalue(b);
		}
		else if(a == 252)
		{
			enc |= b;
			//lcd_gotoxy2(0);
			//lcd_showvalue(UDR);
		}
		//lcd_gotoxy1(8);
		lcd_showvalue(enc);*/
		
	}

/*
	UCSR2B |= (1 << RXEN2) | (1 << TXEN2);
	UCSR2C |=  (1 << UCSZ21)| (1<<UCSZ20)| (1<<UCSZ22);
	UBRR2L=23;
	UBRR2H=0;
	DDRH |= (1<<1);
	DDRH &= ~(1<<0);

	while(1)
	{
		while (!(UCSR2A & (1<<RXC2)));
		if(RXB82 == 0)
		{
			enc |= (UDR2 << 8);
		}
		else if(RXB82 == 1)
		{
			enc |= UDR2;
		}
		lcd_showvalue(enc);
	}
*/
}	