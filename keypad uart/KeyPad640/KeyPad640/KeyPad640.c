/*
 * KeyPad640.c
 *
 * Created: 03-01-2014 11:09:21
 *  Author: Coder Rajat
 */ 


#include <avr/io.h>
#include "R_LCD128.h"
#include <avr/interrupt.h>

uint8_t GetCommand();

int main(void)
{
	lcd_init();	
	lcd_char('T');
	
	sei();
	UART_INIT();
    while(1)
    {
        lcd_char(GetCommand());
    }
}

uint8_t GetCommand()
{
}

void UART_INIT()
{
	// RX - PH0
	// TX - PH1
	
	// Using Channel 2 of 640 Present at PORTH

}

void SendResponseToSlave(int data)
{

}

ISR(USART2_RX_vect)
{
	
}