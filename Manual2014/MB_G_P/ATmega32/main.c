#include<avr/io.h>
#include<util/delay.h>
#include"LCD.h"
int UBRR_VALUE = 25;

int main()
{
	/*********************-----USART INIT------****************************/
	UBRRH = (unsigned char) (UBRR_VALUE >> 8);	
	UBRRL = (unsigned char) UBRR_VALUE;
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << USBS) | (1 << UCSZ1) | (1 << UCSZ0);
	/********************-----TIMER INIT------****************************/
	TCCR1B = 0x07;
	lcd_init();
	DDRB = 0x00;
	PORTB = 0xFF;
	while(1)
	{
		while(!(UCSRA &(1 <<  UDRE)));
		UDR = TCNT1;
		lcd_showvalue(TCNT1);
	}
}