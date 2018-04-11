/*
 * UART.c
 *
 * Created: 2014-08-09 2:39:23 PM
 *  Author: Samir
 */ 
#define  F_CPU 8000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART_0.h"

void UART_0_Init()
{
	UBRR0H = (unsigned char) (((F_CPU/(BAUD*16UL))-1)>>8) ;
	UBRR0L = (unsigned char) (F_CPU/(BAUD*16UL))-1;
	
	UCSR0A=0x00;
	UCSR0B=0x00;

	UCSR0C=0b10000110;
	// Set frame format: asynchronous, 8data, no parity, 1 stop bit

	UCSR0B=0x18;
	
}
void UART_0_Putc(unsigned char data)
{

	while ( !(UCSR0A & (1<<UDRE0)) );  /* Wait for empty transmit buffer */

	UDR0 = data;         /* Start transmition */

}
void UART_0_Puts(char *str)
{
	
	while( *str != '\0' ){
		
		UART_0_Putc( *str++ );
	}
}
