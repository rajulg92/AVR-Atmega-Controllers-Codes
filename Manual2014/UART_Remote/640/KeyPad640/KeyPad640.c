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
#include "Functions.h"

#define F_CPU 14745600

uint8_t UartReceive();
void UartTransmit(uint8_t);
void UartInit();
int temp;


uint8_t DiscStarted = 0, MainPullyStarted = 0, Pully2Started = 0;
uint8_t RightStarted = 0, LeftStarted = 0;


void ProcessPIND( uint8_t data )
{
		//lcd_cmd(LINE2);
		//lcd_string("PinD");
		//lcd_showvalue(data);
		//lcd_string(" ");	
}

void ProcessPINC( uint8_t data )	// 4 DPDT
{
	//Left Motor Base : PC0-PC1
	//Right Motor Base : PC2-PC3
	//Main Pully Motor : PC4-PC5
	//Pully2 Motor : PC6-PC7
	
	lcd_cmd(LINE1);
	lcd_showvalue(data);
	lcd_string("  ");
	
	if( (!(CHECKBIT(data,BIT(0)))) && (CHECKBIT(data,BIT(1))) )						// LEFT MOTORS
	{
		if (LeftStarted == 0)
		{
			LeftFwd(60);
			LeftStarted = 1;
		}		
	}
	else if ( ((CHECKBIT(data,BIT(0))) && (!(CHECKBIT(data,BIT(1))))) )
	{
		if (LeftStarted == 0)
		{
			LeftRev(60);
			LeftStarted = 1;
		}		
	}
	else if ( ((CHECKBIT(data,BIT(0))) && (CHECKBIT(data,BIT(1)))) )
	{
		if (LeftStarted == 1)
		{
			LeftStop();
			LeftStarted = 0;
		}		
	}	
	
	if( (!(CHECKBIT(data,BIT(2)))) && (CHECKBIT(data,BIT(3))) )						// RIGHT MOTORS
	{
		if (RightStarted == 0)
		{
			RightFwd(60);
			RightStarted = 1;
		}
		
	}
	else if ( ((CHECKBIT(data,BIT(2))) && (!(CHECKBIT(data,BIT(3))))) )
	{
		if (RightStarted == 0)
		{
			RightRev(60);
			RightStarted = 1;
		}		
	}
	else if ( ((CHECKBIT(data,BIT(2))) && (CHECKBIT(data,BIT(3)))) )
	{
		if (RightStarted == 1)
		{
			RightStop();
			RightStarted = 0;
		}
	}
	
	
	if( !(CHECKBIT(data,BIT(4))) && (CHECKBIT(data,BIT(5))) )						// MAIN PULLY MOTORS
	{
		MainPullyStarted = 1;
		MainPullyUp();
	}
	else if ( ((CHECKBIT(data,BIT(4))) && !(CHECKBIT(data,BIT(5)))) )
	{
		MainPullyStarted = 1;
		MainPullyDown();
	}
	else if ( ((CHECKBIT(data,BIT(4))) && (CHECKBIT(data,BIT(5)))) )
	{
		if (MainPullyStarted == 1)
		{
			MainPullyStarted = 0;
			MainPullyStop();
		}
		
	}
	
	
	if( !(CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7))) )						// PULLY 2 MOTORS
	{
		Pully2Started = 1;
		Pully2Up();
	}
	else if ( ((CHECKBIT(data,BIT(6))) && !(CHECKBIT(data,BIT(7)))) )
	{
		Pully2Started = 1;
		Pully2Down();
	}
	else if ( ((CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7)))) )
	{
		if (Pully2Started == 1)
		{
			Pully2Stop();
			Pully2Started = 0;
		}
	}
}

void ProcessPINB( uint8_t data ) 
{
	
}

void ProcessPINA( uint8_t data )	// 1 DPDT
{
	if( !(CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7))) )						// DISC MOTORS
	{
		DiscStarted = 1;
		DiscLeft();
	}
	else if ( ((CHECKBIT(data,BIT(6))) && !(CHECKBIT(data,BIT(7)))) )
	{
		DiscStarted = 1;
		DiscRight();
	}
	else if ( ((CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7)))) )
	{
		if (DiscStarted == 1)
		{
			DiscStop();
			DiscStarted = 0;
		}		
	}		
}

int main(void)
{
	lcd_init();	
	//sei();
	UartInit();
	uint8_t c;
	
	DDRJ = 0xFF;		// PORT FOR MAXON DIRECTIONS
	PORTJ = 0x00;	

    while(1)
    {
		UartTransmit(50);
		ProcessPINA(UartReceive());
		
		UartTransmit(60);
		ProcessPINB(UartReceive());
		
		UartTransmit(70);
		ProcessPINC(UartReceive());
		
		UartTransmit(80);
		ProcessPIND(UartReceive());
    }
}

uint8_t UartReceive()
{
	while ( !( UCSR2A & (1<<RXC2)) );
	return UDR2;
}

void UartInit()
{
//	SETBIT(DDRH,BIT(1));
//	CLEARBIT(DDRH,BIT(0));
	DDRH |= (1<<PH1);
	DDRH &= ~(1<<PH0);
	
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

//ISR(USART2_RX_vect)
//{
	//lcd_showvalue(UDR2);
	//lcd_string(" ");
//}


//Left Motor Base : PC0-PC1
//Right Motor Base : PC2-PC3
//Main Pully Motor : PC4-PC5
//Pully2 Motor : PC6-PC7
//DISC Motor : PA6-PA7

