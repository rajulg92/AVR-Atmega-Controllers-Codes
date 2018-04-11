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
#include "UART_1.h"

void UART_1_Init()
{
	UBRR1H = (unsigned char) (((F_CPU/(BAUD*16UL))-1)>>8) ;
	UBRR1L = (unsigned char) (F_CPU/(BAUD*16UL))-1;
	
	UCSR1A=0x00;
	UCSR1B=0x00;

	UCSR1C=0b10000110;
	// Set frame format: asynchronous, 8data, no parity, 1 stop bit

	UCSR1B=0x98;
	
}
void UART_1_Putc(unsigned char data)
{

	while ( !(UCSR1A & (1<<UDRE1)) );  /* Wait for empty transmit buffer */

	UDR1 = data;         /* Start transmition */

}
void UART_1_Puts(char *str)
{
	
	while( *str != '\0' ){
		
		UART_1_Putc( *str++ );
	}
}

