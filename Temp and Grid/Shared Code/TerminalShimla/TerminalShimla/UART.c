/*
 * UART.c
 *
 * Created: 2014-08-09 2:39:23 PM
 *  Author: Samir
 */ 

#define F_CPU 8000000UL



#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"

void UART_init()
{
	UBRRH = (unsigned char) (((F_CPU/(BAUD*16UL))-1)>>8) ;
	UBRRL = (unsigned char) (F_CPU/(BAUD*16UL))-1;
	
	UCSRA=0x00;
	UCSRB=0x00;

	UCSRC=0b10000110;
	// Set frame format: asynchronous, 8data, no parity, 1 stop bit
	UCSRB=0x98;
}
void UART_putc(unsigned char data)
{

	while ( !(UCSRA & (1<<UDRE)) );  /* Wait for empty transmit buffer */

	UDR = data;         /* Start transmition */

}
void UART_puts(char *str)
{
	
	while( *str != '\0' ){
		
		UART_putc( *str++ );
	}
}

