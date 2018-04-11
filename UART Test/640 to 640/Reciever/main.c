/*
Flash this code in ATMEL ATMEGA640 m/c used as Reciever
 */ 


//#define F_CPU 14745600
#include <util/delay.h>
#include <avr/io.h>
#include "LCD_640.h"

uint8_t UartReceive(void);
void UartTransmit(uint8_t);
void UartInit(void);


void UartInit(void)
{
	DDRH |= (1<<PH1);
	DDRH &= ~(1<<PH0);
	
	PORTH |= (1<<PH1);
	PORTH &= ~(1<<PH0);
	
	UCSR2B |= (1<<RXEN2) | (1<<TXEN2);
	UCSR2C |= (1<<UCSZ20) | (1<<UCSZ21);
	UCSR2A |= (1<<U2X2);
	
	UBRR2 = 191;
	// RX - PJ0
	// TX - PJ1
	
	// Using Channel 2 of 640 Present at PORTH
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSR2A & (1<<UDRE2)) );
	UDR2 = data;
}
uint8_t UartReceive(void)
{
	while ( !( UCSR2A & (1<<RXC2)) );
	return UDR2;
}


int main(void)
{
	lcd_init();
	UartInit();
	while(1)
    {
		lcd_showvalue(UartReceive());
		UartTransmit("a");
		lcd_showvalue(UartReceive());
		UartTransmit("b");
		lcd_showvalue(UartReceive());
		UartTransmit("c");
		lcd_showvalue(UartReceive());
		UartTransmit("d");    
    }
	return 0;
}
