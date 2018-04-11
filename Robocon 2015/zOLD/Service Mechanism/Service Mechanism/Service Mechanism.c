/*
 * Service_Mechanism.c
 *
 * Created: 9/21/2014 7:30:05 PM
 *  Author: YateenKedare
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>

void PWM_Init()
{
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
	DDRE |= (1<<PE3) | (1<<PE4) | (1<<PE5);
	DDRH |= (1<<PH6) | (1<<PH7);
	DDRG |= (1<<PG3);
}

int main(void)
{
	PWM_Init();
	DDRD &= ~(1<<5);
	DDRD &= ~(1<<7);
	DDRJ = 0xFF;
	PORTD &= ~(1<<5);
	PORTD &= ~(1<<7);
	PORTJ = 0x00;
	uint8_t LaunchBit = 0, MotorBit = 1;
    while(1)
    {
		if (bit_is_set(PIND, 7))
		{
			if(LaunchBit == 0)
			{
				_delay_ms(175);
				PORTJ = 0xFF;
				LaunchBit = 1;
				MotorBit = 1;
				_delay_ms(1000);
			}
		}
		else
		{
			LaunchBit = 0;
			PORTJ = 0x00;
		}
        while (bit_is_clear(PIND, 5) && MotorBit == 1)
        {
			OCR3A = 700;
        }
		OCR3A = 0;
		MotorBit = 0;
    }
	return 0;
}