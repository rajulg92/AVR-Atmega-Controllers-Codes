#include<avr/io.h>
void left_fwrd(int x)
{
	OCR1A = x;
	PORTD |= 0b01000000;		
}

void left_back(int x)
{
	OCR1A = 100-x;
	PORTD &= ~(0b01000000);
}

void right_fwrd(int x)
{
	OCR1B = x;
	PORTD |= 0b00001000;
}

void right_back(int x)
{
	OCR1B = 100-x;
	PORTD &= ~(0b00001000);
}

void fwrd(int x)
{
	right_fwrd(x);
	left_fwrd(x);
}

void back(int x)
{
	right_back(x);
	left_back(x);
}

void stop()
{
	OCR1A = 0;
	PORTD |= 0b01000000;			
	OCR1B = 0;
	PORTD |= 0b00001000;
}