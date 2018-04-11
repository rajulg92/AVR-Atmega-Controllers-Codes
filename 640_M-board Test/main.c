#define F_CPU 14745600
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "LCD_640.h"
#define false 0
#define true 1
//#define _cb(x,b) x&b
#define _cb(x,b) x&=~b
#define _sb(x,b) x|=b
#define bit(x) (1 << (x))
#define sen1 0
#define sen2 1
#define sen3 2
#define sen4 3
#define sen5 4
#define pin_sen PINC



void pwm_init(void)
{
	TCCR1A = 0xA8;
	TCCR1B = 0x11;
	ICR1 = 737;

	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR5A = 0xA8;
	TCCR5B = 0x11;
	ICR5 = 737;
}

void port_init(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFE;
	DDRE = 0xFF;
	DDRF = 0xFF;
	DDRG = 0xFF;
	DDRH = 0xFF;
	DDRJ = 0xFF;
	DDRK = 0xFF;
	DDRL = 0xFF;
}
void check_port(void)
{
	for(int i= 0; i < 8; i++)
	{
		PORTA |= (1<<i);
		PORTD |= (1<<i);
		PORTC |= (1<<i);
		PORTJ |= (1<<i);
		PORTH |= (1<<i);
		PORTB |= (1<<i);
		_delay_ms(300);
		PORTA = 0x00;
		PORTD = 0x00;
		PORTC = 0x00;
		PORTJ = 0x00;
		PORTH = 0x00;
		PORTB = 0x00;
	}

}

void start_pwm(void)
{
	OCR1A = 700;
	OCR1B = 700;
	OCR1C = 700;
	OCR3A = 700;
	OCR3B = 700;
	OCR3C = 700;
	OCR4A = 700;
	OCR4B = 700;
	OCR4C = 700;
	OCR5A = 700;
	OCR5B = 700;
	OCR5C = 700;
}

void set_pwm_d_bit(void)
{
	_sb(PORTL, bit(6));
	_sb(PORTL, bit(2));
	_sb(PORTG, bit(2));
	_sb(PORTG, bit(1));
	_sb(PORTG, bit(0));
	_sb(PORTL, bit(1));
	_sb(PORTL, bit(0));
	_sb(PORTG, bit(4));
	_sb(PORTG, bit(3));
	_sb(PORTH, bit(7));
	_sb(PORTH, bit(6));
	PORTL |= (1<< 7);

}
void stop_pwm(void)
{
	OCR1A = 0;
	_cb(PORTL, bit(2));
	OCR1B = 0;
	_cb(PORTL, bit(6));
	OCR1C = 0;
	PORTL &= ~(1<< 7);
	OCR3A = 0;
	_cb(PORTH, bit(6));
	OCR3B = 0;
	_cb(PORTH, bit(7));
	OCR3C = 0;
	_cb(PORTG, bit(3));
	OCR4A = 0;
	_cb(PORTG, bit(4));
	OCR4B = 0;
	_cb(PORTL, bit(0));
	OCR4C = 0;
	_cb(PORTL, bit(1));
	OCR5A = 0;
	_cb(PORTG, bit(0));
	OCR5B = 0;
	_cb(PORTG, bit(1));
	OCR5C = 0;
	_cb(PORTG, bit(2));
}

void check_pwm(void)
{
	start_pwm();
	_delay_ms(100);
	set_pwm_d_bit();
	_delay_ms(1000);
	stop_pwm();
	_delay_ms(1000);
}

void timer_init(void)
{
	/*--------Timer Initialisation-------*/
	TCCR1B = 0x07;
	DDRD &= ~(1<<6);
	PORTD |= (1<<6);		
}

int main(void)
{
	//lcd_init();
	pwm_init();
	port_init();
	stop_pwm();
	PORTD = 0xFE;
	OCR3A = 700 ;
	OCR3B = 700*0.9;
	OCR3C = 700*0.8;
	OCR4A = 700*0.7 ;
	OCR4B = 700*0.6;
	OCR4C = 700*0.5;
	OCR1A = 700*0.4 ;
	OCR1B = 700*0.3;
	OCR1C = 700*0.2;
	OCR5A = 700*0.15 ;
	OCR5B = 700*0.12;
	OCR5C = 700*0.10;
	PORTH |= (1<<7);
	//_delay_ms(1000);
	while(1);
	/*timer_init();
	int a = 0;
	TCNT1 = 0;
	
	
/*	while(1)
	{
	
		OCR3A = 700*0.5;
		OCR3B = 700;
		_cb(PORTH, bit(6));
		_cb(PORTH, bit(7));
		lcd_gotoxy1(0);
		lcd_showvalue(TCNT1);
		lcd_string("    ");
		if ((TCNT1 >= 0)  &&  (TCNT1 <= 4000))		a++;
		lcd_gotoxy2(0);
		lcd_showvalue(a);
		lcd_char(' ');
		/*_delay_ms(5000);
		set_pwm_d_bit();
		_delay_ms(5000);
		*/
/*		
	}
	
	while(1)
	{
		do {
		
			i = i+20;
			OCR3A = 737*i/100;
			_delay_ms(5000);
			OCR3A = 0;
			_delay_ms(5000);
		}while(i<101);
		i = 20;
	}
	
	while(1)
	{
		_sb(PORTB, bit(3));
		_delay_ms(1000);
		_sb(PORTB, bit(4));
		_delay_ms(1000);
		PORTB = 0x00;
		_delay_ms(1000);
		PORTA = 0xFF;
	}
	while(1)
	{
		//check_port();
		//check_pwm();
	}*/
	return 0;
}