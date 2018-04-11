/*
 * ServoHand.c
 *
 * Created: 9/19/2013 6:34:57 PM
 *  Author: YateenKedare
 */ 


#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	
	TCCR1A = 0b10100000;
	TCCR1B = 0b00010001;
	
	
	ICR1 =  10000;
	OCR1A = 2500;
	OCR1B = 10000;
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}