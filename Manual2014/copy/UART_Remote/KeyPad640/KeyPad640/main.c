 /*
	115 - straight
	420 - curve
	410 - seasaw to pole
	
	3.35cm - dia of enc wheel
 */


#include <avr/io.h>
#include "R_LCD640.h"
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "PWM640.h"
//#include "Functions.h"

#define F_CPU 14745600

uint8_t UartReceive(void);
void UartTransmit(uint8_t);
void UartInit(void);
int temp,speed = 20;

#define sen1 5		//left extreme
#define sen2 4
#define sen3 1
#define sen4 2
#define sen5 3		//right extreme
#define PIN_sen PINC
#define speed 600
uint8_t DiscStarted = 0, MainPullyStarted = 0, Pully2Started = 0;
uint8_t RightStarted = 0, LeftStarted = 0;
uint8_t PidStarted = 0;

uint8_t data = 0, err = 0, acc_err = 0, inst_err = 0, prev_err = 0;
float p = 17, i = 0.01, d = -0.01;
float corr = 0;

uint8_t EncoderStarted = 0;

void PWM_init(void)
{
	TCCR1A = 0xA8;
	TCCR1B = 0x11;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	TCCR5A = 0xA8;
	TCCR5B = 0x11;
	ICR3 = 737;
	ICR4 = 737;
	ICR1 = 737;
	ICR5 = 737;
}

void port_init(void)
{
	DDRJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3) | (1 << PJ4) | (1 << PJ5) ;
	DDRE |= (1<<PE3) | (1<<PE4);
	DDRG |= (1 << PG4) | (1<<PG3);
	DDRL |= (1 << PL0) | (1<<PL2);
	DDRA |= (1<<PA0) |(1<<PA1) | (1<<PA2) | (1<<PA3) | (1 << PA4) | (1 << PA5) | (1 << PA6) | (1 << PA7);
	DDRC = 0x00;
	PORTC = 0xFF;
	PORTJ &= ~(1 << PJ4) ;
	PORTJ &= ~(1 << PJ5);
}

void LeftFwd(int x) {
	//PORTJ &= ~(1<<PJ5);
	OCR3B = x;
	PORTJ |= (1 << PJ2);
	PORTJ &= ~(1 << PJ3);
}

void LeftRev(int x) {
	//PORTJ &= ~(1<<PJ5);
	OCR3B = x;
	PORTJ |= (1 << PJ3);
	PORTJ &= ~(1 << PJ2);
}

void RightFwd(int x) {
//PORTJ &= ~(1<<PJ4);
	OCR3A = x;
	PORTJ |= (1 << PJ1);
	PORTJ &= ~(1 << PJ0);
}

void RightRev(int x) {
	//PORTJ &= ~(1<<PJ4);
	OCR3A = x;
	PORTJ |= (1 << PJ0);
	PORTJ &= ~(1 << PJ1);
}

void LeftStop(void) {
	OCR3B = 145;
	//PORTJ |= (1<<PJ5);
}

void RightStop(void) {
	//PORTJ |= (1<<PJ4);
	OCR3A = 145;
}

void DiscStop(void)
{
	OCR4A = 0;
	PORTG ^= (1<<PG4);
}

void DiscRight(uint8_t s)
{
	OCR4A = s;
	PORTG |= (1<<PG4);
}

void DiscLeft(uint8_t s)
{
	OCR4A = s;
	PORTG &= ~(1<<PG4);
}

void MainPullyStop(void)
{
	OCR4B = 40;
	PORTL &= ~(1<<PL0);
}

void MainPullyUp(void)
{
	OCR4B = 100;
	PORTL &= ~(1<<PL0);
}

void MainPullyDown(void)
{
	OCR4B = 100;
	PORTL |= (1<<PL0);
}

void ExecuteSeeSaw(void)
{
	PORTA |= (1<<PA3);
	for(int i = 0; i < 3; i++)
	{
		PORTA |= (1<<PA1);
		_delay_ms(1000);
		PORTA &= ~(1<<PA1);
		_delay_ms(1000);
	}
	PORTA &= ~(1<<PA3);
	PORTA &= ~(1<<PA1);
	
}

void ADCInit(void)
{
	ADCSRA |= (1<<ADEN);
	ADMUX |= (1<<MUX1);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= (1<<ADLAR);
	ADCSRA |= (1<<ADSC);
}

int ADCConvert(void)
{
	ADCSRA |= (1<<ADSC);
	return ADCH;	
}



void pid(void)
{
	//get the sensor input

	if(bit_is_set(PIN_sen, sen1))
		err = 3;
	else if(bit_is_set(PIN_sen, sen2))
		err = 2.4;
	else if(bit_is_set(PIN_sen, sen3))
		err = 2.2;
	else if(bit_is_set(PIN_sen, sen4))
		err = 2;
	else if(bit_is_set(PIN_sen, sen5))
		err = 1;
	else
		err = -0.1;
	
	
	inst_err = err - prev_err;
	corr = p*err + i*acc_err + d*inst_err;
	acc_err += err;
	prev_err = err;

	PidStarted = 1;

	CLEARBIT(PORTJ,BIT(1));		// Direction Bits Left Base
	SETBIT(PORTJ,BIT(0));
	SETBIT(PORTJ,BIT(3));		// Direction Bits Right Base
	CLEARBIT(PORTJ,BIT(2));
	
	OCR3B = 350 + (int)(ceil(corr));
	OCR3A = 400 - (int)(ceil(corr));
}

void ProcessADC(uint8_t analogData)
{
/*	speed = 100+ceil(analogData*2.4);
	if (speed >=650 )
	{
		speed = 650;
	}
*/	//speed = 600;
}

void ProcessPIND( uint8_t data )		// 1 DPDT of DISC	
{		
		if( !(CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7))) )						// DISC MOTORS
		{
			DiscStarted = 1;
			DiscRight(100);
		}
		else if ( ((CHECKBIT(data,BIT(6))) && !(CHECKBIT(data,BIT(7)))) )
		{
			DiscStarted = 1;
			DiscLeft(100);
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

void ProcessPINC( uint8_t data )	// 4 DPDT
{
	//Left Motor Base : PC0-PC1
	//Right Motor Base : PC2-PC3
	//Main Pully Motor : PC4-PC5
	//Pully2 Motor : PC6-PC7
	if( (!(CHECKBIT(data,BIT(0)))) && (CHECKBIT(data,BIT(1))) )						// LEFT MOTORS
	{
			LeftFwd(speed);
			LeftStarted = 1;
	}
	else if ( ((CHECKBIT(data,BIT(0))) && (!(CHECKBIT(data,BIT(1))))) )
	{
			LeftRev(speed);
			LeftStarted = 1;
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
			RightRev(speed);
			RightStarted = 1;
	}
	else if ( ((CHECKBIT(data,BIT(2))) && (!(CHECKBIT(data,BIT(3))))) )
	{
			RightFwd(speed);
			RightStarted = 1;
	}
	else if ( ((CHECKBIT(data,BIT(2))) && (CHECKBIT(data,BIT(3)))) )
	{
		if (RightStarted == 1)
		{
			RightStop();
			RightStarted = 0;
		}
	}
	
	
	if( (!(CHECKBIT(data,BIT(4)))) && (CHECKBIT(data,BIT(5))) )						// MAIN PULLY MOTORS
	{
			MainPullyStarted = 1;
			MainPullyUp();
	}
	else if ( ((CHECKBIT(data,BIT(4))) && (!(CHECKBIT(data,BIT(5))))) )
	{
			MainPullyStarted = 1;
			MainPullyDown();
	}
	else if ( ((CHECKBIT(data,BIT(4))) && (CHECKBIT(data,BIT(5)))) )
	{
			MainPullyStarted = 0;
			MainPullyStop();
	}
	
	/*
	if( !(CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7))) )						// PULLY 2 MOTORS
	{
		if (Pully2Started == 0)
		{
			Pully2Started = 1;
			Pully2Up();
		}
		
	}
	else if ( ((CHECKBIT(data,BIT(6))) && !(CHECKBIT(data,BIT(7)))) )
	{
		if (Pully2Started == 0)
		{
			Pully2Started = 1;
			Pully2Down();
		}		
	}
	else if ( ((CHECKBIT(data,BIT(6))) && (CHECKBIT(data,BIT(7)))) )
	{
		if (Pully2Started == 1)
		{
			Pully2Stop();
			Pully2Started = 0;
		}
	}*/
}

void ProcessPINB( uint8_t data ) 
{
	
}

void ProcessPINA( uint8_t data )		// PID + PNEUMATICS (data Switches)
{
	if ( !(CHECKBIT(data,BIT(0))) )
	{
		pid();
	}
	else if ((CHECKBIT(data,BIT(0))))
	{
		if(PidStarted == 1)
		{
			PidStarted = 0;
			LeftStop();
			RightStop();
		}
	}
	
	
	if ( !(CHECKBIT(data,BIT(1))))
	{
		ExecuteSeeSaw();
	}
	
	if (!(CHECKBIT(data,BIT(2))))
	{
		EncoderStarted = 1;
		
		if(TCNT1 <= 200)
		{
			LeftFwd(30);
			RightFwd(30);
		}
		
		if(TCNT1 >=200 && TCNT1 <= 450)
		{
			RightFwd(68);
			LeftFwd(35);
		}
		
		if(TCNT1 >= 450 && TCNT1 <= 1024)
		{
			LeftFwd(44 + TCNT1/20);
			RightFwd(58 + TCNT1/55);
		}
	
		
		if (TCNT1 >= 1024)
		{
			LeftStop();
			RightStop();
		}

/*		

		if (TCNT1 < 870)
		{
			LeftFwd(44 + TCNT1/50);
			RightFwd(58 + TCNT1/30);
		}
		if (TCNT1 >= 870)
		{
			LeftStop();
			RightStop();
		}
		
*/
	}
	if ((CHECKBIT(data,BIT(2))))
	{
		if (EncoderStarted == 1)
		{
			EncoderStarted = 0;
			LeftStop();
			RightStop();
		}
	}
	
/*	if (!(CHECKBIT(data,BIT(3))))		// RED SWITCH (For Auto Reset)		// PA3
	{
		TCNT1 = 0;
		EncoderStarted = 0;
		
		
	}
*/
}

int main(void)
{
	lcd_init();
	port_init();

	PWM_init();
	
	//sei();
	UartInit();
	ADCInit();
	
	DDRJ = 0xFF;		// PORT FOR MAXON DIRECTIONS
	PORTJ = 0x00;
	
	DDRC = 0x00;		// Sensor Array	of 5 and PC0 Pencil Sensor
	PORTC = 0xFF;
	
	DDRF |= (1<<PF0);
	PORTF &= ~(1<<PF0);
	
	CLEARBIT(DDRD,BIT(6));		// Encoder Port Initialization
	SETBIT(PIND,BIT(6));
	
	TCCR1B = 0x07;		// Timer/Counter 1 Initialization for Encoder Count
	TCNT1 = 0;
	
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
		
		UartTransmit(90);
		ProcessADC(UartReceive());
		
	}
}

uint8_t UartReceive(void)
{
	while ( !( UCSR2A & (1<<RXC2)) );
	return UDR2;
}

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

//Left Motor Base : PC0-PC1
//Right Motor Base : PC2-PC3
//Main Pully Motor : PC4-PC5
//Pully2 Motor : PC6-PC7
//DISC Motor : PA6-PA7

	//Pencil Sensor --> PC0
	//Encoder --> PD6