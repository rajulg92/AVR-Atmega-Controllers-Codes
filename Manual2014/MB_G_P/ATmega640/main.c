/*The useless stuff for Rajul, Yateen
  How we will be implementing
  See for the robot to move in the straight line the speed of both the wheels should be 
  constant...Which we are giving by keeping the OCR same for both the wheels
  So, first we will try to move it dead straight line 

  For the curve tracing part I am thinking of sampling the encoder values at minute instants of timw and storing them in the 
  array then check the current error against the previously stored value...If the error is not equal to zero then we take the 	corrective action using our PID algorithm
  So, we require a timer to sample the values and also to check them at various instants
  Next we require a array to keep track of the error values at different instants of time
  Then we need the usual PID stuff and the obtain the correction factor and take the corresponding factor 
  The only problem I see here is the slippage 
  Feel free to add anything
 */
//This is the code for the straight line
/*
ENCODER I/P FROM ATMEGA32 RXD2

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "LCD640.h"


void port_init();
void pwm_init();
void timer_ini();
void uart_ini();
void trace_circular_curve();

int UBRR_VALUE = 25;
uint16_t enc1, enc2;
void port_init()
{
//	DDRA |= (1 << PA1) | (1 << PA3) | (1 << PA4) ;
//	DDRA &= ~ (1 << PA2);
	DDRH |= (1 << PH4) | (1 << PH5);		//OC3B	//OC3A	DIRECTION BITS
	DDRL |= (1<<PL0) | (1<<PL1);	//OC3A, 3B O/P DECLARATION
}

void pwm_init()
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
}

void timer_ini()
{
	TCCR1B = 0x07;
	DDRD |= (1<<6);
}


void uart_ini()
{
	UBRR2H = (unsigned char) (UBRR_VALUE >> 8);	
	UBRR2L = (unsigned char) UBRR_VALUE;
	UCSR2B |= (1 << RXEN2) | (1 << TXEN2);
	UCSR2C |= (1 << USBS2) | (1 << UCSZ12) | (1 << UCSZ02);

}

void trace_circular_curve()
{
	//outer s == 490	57.10
	//inner s == 429	50
	//pwm ratio	1.1421
	while(enc1 <= 490 && enc2<=429)
	{
		OCR4B = 420;
		OCR4C = 368;
	}
}

int main()
{
	
	port_init();
	pwm_init();
	timer_ini();
	lcd_init();
	//enc1 is connected to right wheel
 //values from the two encoders
	//for straight line this should be zero if I am guaranteed that they are moving with the same speed
	//sei();
	double kp = 0, ki = 0, kd = 0;
	int8_t acc_err = 0, prev_err = 0, inst_err;
	int bal;
	ICR4 = 737;
/*	OCR4B = 300;
	OCR4C = 300;
	*/
	
	while(1) {
	/*PORTH |= (1 << PH4) | (1 << PH5);		//OC3B	//OC3A	DIRECTION BITS
	PORTL |= (1<<PL0) | (1<<PL1);
	*/
		lcd_cmd(0x01);
		PORTL |= (1<<PL0) | (1<<PL1);
		OCR4B = 500;
		OCR4C = 500;
		lcd_string("FWRD");
		_delay_ms(2000);
		
		lcd_cmd(0x01);
		PORTL &= ~(1<<PL0) ;
		PORTL &= ~(1<<PL1);
		OCR4B = 0;
		OCR4C = 0;
		lcd_string("STOP");
		_delay_ms(2000);
		
		lcd_cmd(0x01);
		PORTL &= ~(1<<PL0);
		PORTL &= ~(1<<PL1);
		OCR4B = 500;
		OCR4C = 500;
		lcd_string("REV");
		_delay_ms(2000);
		
		lcd_cmd(0x01);
		PORTL |= (1<<PL1);
		PORTL |= (1<<PL0);
		OCR4B = 0;
		OCR4C = 0;
		lcd_string("STOP");
		_delay_ms(2000);
		//to write the code for reception from two encoders
/*		enc1 = TCNT1;
		uint16_t diff = enc1-enc2;
		inst_err = diff - prev_err;
		//if the difference is positive then the right wheel is moving faster than the left wheel
		//if the difference is negative then left wheel is moving faster than the right wheel
		//I need to balance the two

		bal = kp*diff + ki*acc_err + kd*inst_err;
		acc_err += diff;
		prev_err = inst_err;
		
		OCR4B = OCR4B - ceil(bal);
		OCR4C = OCR4C + ceil(bal);*/
	}
	return 0;
}
