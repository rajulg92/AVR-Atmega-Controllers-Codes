#include<avr/io.h>
#include<util/delay.h>
#include<interrupt.h>
//#include "LCD640.h"
#include"LCD.h"
void main()
{	
	lcd_init();
	UCSRB = 0xFC ;
	UCSRC |=  (1 << UCSZ1)| (1<<UCSZ0);
	UBRRL=25;
	UBRRH=0;
	DDRD |= (1<<1);
	DDRD &= ~(1<<0);

	while(1)
	{
	//	lcd_cmd(1);
		
		lcd_gotoxy1(0);
		UCSRB |= (1<<TXB8);
		while(!(UCSRA &(1 <<  UDRE)));
		UDR = 4;
		lcd_showvalue(UDR);
		lcd_gotoxy1(8);
		lcd_showvalue(UCSRB);
		
		lcd_gotoxy2(0);
		UCSRB &= ~(1<<TXB8);
		while(!(UCSRA &(1 <<  UDRE)));
		UDR = 0;
		lcd_showvalue(UDR);
		lcd_gotoxy2(8);
		lcd_showvalue(UCSRB);
		//_delay_ms(100);
	}
}	