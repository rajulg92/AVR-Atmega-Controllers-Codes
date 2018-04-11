/*
Right encoder on pd6(T1) on atmega640 enc_r
OCR4B RIGHT
OCR4C LEFT
*/
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include "LCD640.h"

/*-------Variable initialisation---------*/
//int error, i, temp_b = 0, temp_c = 0, a = 0, b = 0,x;
/*uint16_t enc_r = 0;
uint16_t enc_l = 0;
int acc_err = 0, inst_err = 0, prev_err = 0, error = 0, bal = 0,x;
float kp = 10, ki = 0.8, kd = 0.7;




void pwm_init(void)
{
	TCCR4B = 0x11;
	ICR4 = 737;
}
*/

void port_init(void)
{
	DDRH |= (1 << PH4) | (1 << PH5);		//OC3B	//OC3A	DIRECTION BITS
	DDRL |= (1<<PL0)   | (1<<PL1);	//OC3A, 3B O/P DECLARATION
}
void timer_init(void)
{
	/*--------Timer Initialisation-------*/
	TCCR1B = 0x07;
	DDRD &= ~(1<<6);
	PORTD |= (1<<6);		
}
/*

void usart_init(void)
{
	DDRH |= (1<<1);
	DDRH &= ~(1<<0);
	/*------------USART INIT------------*/
/*	UCSR2A |= _BV(1);
	UCSR2B |= (1 << RXEN2) | (1 << TXEN2);
	UCSR2C |=  (1 << UCSZ21)| (1<<UCSZ20);
	UBRR2L=191;
	UBRR2H=0;
	
}
void USART_Flush(void)
{
	unsigned char dummy;
	while ( UCSR2A & (1<<RXC2) ) 
	dummy = UDR2;
}

void receive_encr()
{
		while(!(UCSR2A &(1 <<  UDRE2)));
        UDR2 = 2;
        lcd_gotoxy1(0);
        lcd_string("Request1 Sent");
        USART_Flush();
        
        while (!(UCSR2A & (1<<RXC2)));
		b = UDR2;
        enc_r |= (b<<8);
        lcd_gotoxy1(0);
        lcd_string("Received data");
        USART_Flush();
		
       /* while(!(UCSR2A &(1 <<  UDRE2)));
        UDR2 = 2;
        lcd_gotoxy1(0);
        lcd_string("Request2 Sent");
		USART_Flush();
		*/
 /*       while (!(UCSR2A & (1<<RXC2)));
		b = UDR2;
        enc_r |= b;
        lcd_gotoxy1(0);
        lcd_string("Received data");
		USART_Flush();
}

void PID()
{
	/*--------------PID----------------*/
/*	error = (enc_r - enc_l);
	
	inst_err = error - prev_err;
	bal = kp*error + ki*acc_err + kd*inst_err;
	prev_err = error;
	acc_err += error;

	
	temp_b = OCR4B + (ceil(bal)/50);
	temp_c = OCR4C - (ceil(bal)/50);
	
	if(temp_b < 100)
	{
		temp_b = 100;
	}
	else if(temp_b > 730)
	{
		temp_b = 730;
	}
	if(temp_c < 100)
	{
		temp_c = 100;
	}
	else if(temp_c > 730)
	{
		temp_c = 730;
	}
	OCR4B = temp_b;
	OCR4C = temp_c;
}*/

int main(void)
{
	DDRC |= (1<<6) | (1<<0);
	PORTC |= (1<<6)| (1<<0);
	lcd_string("hel");
	/* ----------Calling all init functn--------- */
	lcd_init();
	port_init();
	//pwm_init();
	timer_init();
	//usart_init();
	
	/*------TP Code because shit happens------*/

//	PORTL |= (1<<PL0);
//	PORTL |= (1<<PL1);
	

	
	while(1)
	{
		
		//receive_encr();
	//	enc_l = TCNT1;
		
        lcd_gotoxy1(0);
		lcd_showvalue(TCNT1);
		lcd_string("    ");
    /*    lcd_gotoxy2(0);
		lcd_showvalue(enc_l);
		
		lcd_gotoxy1(8);
		lcd_showvalue(OCR4B);
		lcd_gotoxy1(0);
		lcd_showvalue(OCR4C);
		
		PID();	
*/	}
	return 0;
}

