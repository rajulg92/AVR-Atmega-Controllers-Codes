/*
 * KeyPad640.c
 *
 * Created: 03-01-2014 11:09:21
 *  Author: Coder Rajat
 */ 


#include <avr/io.h>
#include "R_LCD640.h"
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "PWM640.h"


#define F_CPU 14745600

uint8_t GetCommand();
int temp;

int main(void)
{
	lcd_init();	
	
	sei();

	UART_INIT();
	//SendResponseToSlave(100);
    while(1)
    {
		//while ( !( UCSR2A & (1<<RXC2)) );
		//temp=UDR2;
		//lcd_gotoxy1(0);
		//lcd_char(temp);
		//lcd_showvalue(temp);
    }
}

uint8_t GetCommand()
{
	
}

void UART_INIT()
{
//	SETBIT(DDRH,BIT(1));
//	CLEARBIT(DDRH,BIT(0));
	DDRH |= (1<<PH1);
	DDRH &= ~(1<<PH0);
	
	UCSR2B |= (1<<RXEN2) | (1<<TXEN2) | (1<<RXCIE2);
	UCSR2C |= (1<<UCSZ20) | (1<<UCSZ21);
	UCSR2A |= (1<<U2X2);
	
	UBRR2H = 0;
	UBRR2L = 191;
	
	// RX - PJ0
	// TX - PJ1
	
	// Using Channel 2 of 640 Present at PORTH
}

void SendResponseToSlave(uint8_t data)
{
	while ( !( UCSR2A & (1<<UDRE2)) );
	UDR2 = data;	
}

ISR(USART2_RX_vect)
{	
	uint8_t c = UDR2;
	
	//lcd_char(c);
	//lcd_showvalue(c);
	
	if(c == '$')
	{
		SendResponseToSlave(100);
		_delay_ms(10);
	}
	
	
	else if (c >= 30 && c <= 39)			// PATH TESTING MODULE
	{
		lcd_showvalue(c);
		SendResponseToSlave(150);
		_delay_ms(10);
		
		switch(c)
		{
			case 30:
			SendResponseToSlave(220);
			break;
			
			case 31:
			Ptest1();
			SendResponseToSlave(220);
			break;
			
			case 32:
			Ptest2();
			SendResponseToSlave(220);
			break;
			
			case 33:
			Ptest3();
			SendResponseToSlave(220);
			break;
			
			case 34:
			Ptest4();
			SendResponseToSlave(220);
			break;
			
			case 35:
			Ptest5();
			SendResponseToSlave(220);
			break;
			
			case 36:
			Ptest6();
			SendResponseToSlave(220);
			break;
			
			case 37:
			Ptest7();
			SendResponseToSlave(220);
			break;
			
			case 38:
			Ptest8();
			SendResponseToSlave(220);
			break;
			
			case 39:
			Ptest9();
			SendResponseToSlave(220);
			break;
		}
		_delay_ms(100);
		lcd_cmd(1);
		lcd_string("Command Executed");
		lcd_cmd(LINE2);
		lcd_string("Successfully...");
		_delay_ms(100);
	}	
	
	else if (c >= 10 &&  c<= 19)			// UNIT TESTING MODULE
	{
		SendResponseToSlave(150);
		_delay_ms(10);
				
		switch(c)
		{
			case 10:
			SendResponseToSlave(220);
			break;
			
			case 11:
			Utest1();
			SendResponseToSlave(220);
			break;
			
			case 12:
			Utest2();
			SendResponseToSlave(220);
			break;
			
			case 13:
			Utest3();
			SendResponseToSlave(220);
			break;
			
			case 14:
			Utest4();
			SendResponseToSlave(220);
			break;
			
			case 15:
			Utest5();
			SendResponseToSlave(220);
			break;
			
			case 16:
			Utest6();
			SendResponseToSlave(220);
			break;
			
			case 17:
			Utest7();
			SendResponseToSlave(220);
			break;
			
			case 18:
			Utest8();
			SendResponseToSlave(220);
			break;
			
			case 19:
			Utest9();
			SendResponseToSlave(220);
			break;
		}
		_delay_ms(100);
		lcd_cmd(1);
		lcd_string("Command Executed");
		lcd_cmd(LINE2);
		lcd_string("Successfully...");
		_delay_ms(100);
	}
	
}

void Utest1()
{
	lcd_cmd(1);
	lcd_string("Executing --U1--");	
}

void Utest2()
{
	//lcd_cmd(1);
	//lcd_string("Executing --U2--");
	PWM('4','B',10000,100,SET);
	PWM('4','C',10000,100,SET);
}

void Utest3()
{
	lcd_cmd(1);
	lcd_string("Executing --U3--");
}

void Utest4()
{
	lcd_cmd(1);
	lcd_string("Executing --U4--");
}

void Utest5()
{
	//lcd_cmd(1);
	//lcd_string("Executing --U5--");
	PWM('4','B',10000,0,TOGGLE);
	PWM('4','C',10000,0,TOGGLE);	
}

void Utest6()
{
	lcd_cmd(1);
	lcd_string("Executing --U6--");
}

void Utest7()
{
	lcd_cmd(1);
	lcd_string("Executing --U7--");
}

void Utest8()
{
	//lcd_cmd(1);
	//lcd_string("Executing --U8--");
	PWM('4','B',10000,100,CLEAR);
	PWM('4','C',10000,100,CLEAR);
}

void Utest9()
{
	lcd_cmd(1);
	lcd_string("Executing --U9--");
}

void Ptest1()
{
	lcd_cmd(1);
	lcd_string("Executing --P1--");
}

void Ptest2()
{
	lcd_cmd(1);
	lcd_string("Executing --P2--");
}

void Ptest3()
{
	lcd_cmd(1);
	lcd_string("Executing --P3--");
}

void Ptest4()
{
	lcd_cmd(1);
	lcd_string("Executing --P4--");
}

void Ptest5()
{
	lcd_cmd(1);
	lcd_string("Executing --P5--");
}

void Ptest6()
{
	lcd_cmd(1);
	lcd_string("Executing --P6--");
}

void Ptest7()
{
	lcd_cmd(1);
	lcd_string("Executing --P7--");
}

void Ptest8()
{
	lcd_cmd(1);
	lcd_string("Executing --P8--");
}

void Ptest9()
{
	lcd_cmd(1);
	lcd_string("Executing --P9--");
}