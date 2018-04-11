/*
 * Data Freeze Prototype.c
 *
 * Created: 12-05-2016 16:06:12
 * Author : RajulGupta
 */ 
/******************************************Code Summary************************************/
/*
1> This code is used to control the Relay in the DataFreeze PCB for Resetting the mCS PCB Stack.
So, As soon as TRIGGER is received on pin PB0 of ATTINY85 IC it resets the PCB Stack by toggling 
PB1 as o/p to Relay. 
2> Fuse Bits A2 5F
*/


# define F_CPU 8000000UL

#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>

int main(void)
{
	DDRB &= ~(1<<DDB0); //declaring PB0 as input
	DDRB |= (1<<DDB1);//declaring PB1 as Output
	
	sbi(PORTB,0);//Pull up at PB0
	cbi(PORTB,1);//Relay is off initially
	
	/* Replace with your application code */
    while (1) 
    {
		if (!(PINB & (1<<PINB0)))
		{
			sbi(PORTB,1);// Relay ON
			_delay_ms(5000);
			cbi(PORTB,1);// Relay OFF
			_delay_ms(5000);
		}
    }
}

