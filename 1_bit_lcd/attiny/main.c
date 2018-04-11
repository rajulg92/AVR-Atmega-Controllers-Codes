#include<avr/io.h>
#include "LCD.h"
void UartInit(void)
{	
	DDRD &= ~(1<<PD0);
	DDRD |= (1<<PD1);
	
	UCSRA |= (1<<U2X);
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0) | (1<<USBS);
	UBRRL = 12;
	UBRRH = 0;
}

uint8_t UartReceive(void)
{
	while(!(UCSRA & (1<<RXC)) );
	return UDR;
}

void UartTransmit(uint8_t data)
{
	while (!( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void USART_Flush( void )
{
	unsigned char dummy;
	while(UCSRA & (1<<RXC)) 
		dummy = UDR;
}

int main(void)
{
	lcd_init();
	UartInit();
	lcd_string("Ready");
	
	while(1);
	{
		USART_Flush();
		lcd_string("done");
		UartTransmit(123);
		lcd_string("done");
	}
}