#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "LCD640.h"
#include "joystick.h" 

int i_data1 = 0,flag1=0,flag2=0,flag=0,flagd=0;

void pwm_init()
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 =737;
}

void port_init()
{
	DDRH |= (1 << PH4) | (1 << PH5) | (1<<PH3);		//OC3B	//OC3A	DIRECTION BITS
	DDRL |= (1<<PL0)   | (1<<PL1);	//OC3A, 3B O/P DECLARATION
	DDRG |= (1<<PG4) | (1<<PG3);
	DDRE |= (1<<PE5);
}

void leftREV(int x)
{
	PORTL &= ~(1<<1);
	OCR4C = x;
}

void rightREV(int x)
{
	PORTL &= ~(1<<0);
	OCR4B = x;	
}
void leftFWD(int x)
{
	PORTL |= (1<<1);
	OCR4C = x;
}
void rightFWD(int x)
{
	PORTL |= (1<<0);
	OCR4B = x;
}

void discclock()
{
	OCR4A = 737;
	PORTG |= (1<<PG4);
}

void discanticlock()
{
	OCR4A = 737;
	PORTG &= ~(1<<PG4);
}

void pullyup()
{
	OCR3C = 737;
	PORTG |= (1<<PG3);
	flag=1;
	flagd=0;

}

void pullydown()
{
	OCR3C = 737;
	PORTG &= ~(1<<PG3);
	flag=1;
	flagd=1;
	
}

void stop2()
{
	OCR4A = 0;
	//OCR3C=0;
	if(flag==1)
	{
		PORTG ^= (1<<PG3);
		flag=0;
	}
	if(flagd=1)
	{
		PORTG |= (1<<PG3);
		OCR3C=35;
	}
	else
	{
		OCR3C=0;
	
	}
}

void bot_stop2()
{
	OCR4B = 0;
	OCR4C = 0;
	PORTH &= ~(1<<PH4);
	PORTH &= ~(1<<PH5);
}
void bot_stop()
{
	OCR4B = 0;
	OCR4C = 0;
	PORTL ^= _BV(0) ^ _BV(1);
}

int main(void)
{
	lcd_init();
	ps2_init();
	port_init();
	pwm_init();

	joystick_config();
	joystick_config();
	joystick_config();
	
	while(1)
	{
		joystick_poll();
		lcd_gotoxy1(0);
		lcd_showvalue(data0);
		lcd_gotoxy1(5);
		lcd_showvalue(data2);
		lcd_gotoxy1(103);
		lcd_showvalue(data3);
		lcd_gotoxy2(0);
		lcd_showvalue(data4);
		lcd_gotoxy2(8);
		lcd_showvalue(data5);
	}
	
	while(1)
	{
		joystick_poll();
		
		//lcd_gotoxy2(0);
		//lcd_showvalue(data1);
		//lcd_gotoxy1(0);
		switch(data0)
	    {
			case 48:	//63
				leftREV(700);
				rightREV(700);
				lcd_string("REVERSE");
			break;
			case 0:	//15
				leftFWD(700);
				rightFWD(700);
				lcd_string("FORWARD");
			break;
			case 16:	//31
				leftFWD(737);
				rightFWD(500);
				//PORTH &= ~(1<<PH4);
				lcd_string("RIGHT");
			break;	
			case 112:	//127
				leftFWD(500);
				rightFWD(737);
				lcd_string("LEFT");
			break;
			case 224:
				bot_stop2();
				lcd_string("STOP");
			break;
			case 192:
				bot_stop2();
				lcd_string("STOP");
			break;
			case 240:
				bot_stop2();
				lcd_string("STOP");
			break;
			default:
				/*if(i_data1 == 48 || i_data1 == 15 || i_data1 == 16 || i_data1 == 112)
					bot_stop();
				else
					bot_stop2();
				//lcd_string("STOP");*/
				flag1=1;
				bot_stop2();
				lcd_string("STOP");
			break;
		}
		i_data1 = data0;
		switch(data1)
		{
			case 131:
			case 195:
				PORTH |= (1<<PH4);
				PORTH |= (1<<PH5);
				leftREV(700);
				rightFWD(700);
			break;
			case 135:
				PORTH |= (1<<PH4);
				PORTH |= (1<<PH5);
				leftFWD(700);
				rightREV(700);
			break;
			case 31:
				discclock();
			break;
			case 127:
				discanticlock();
			break;
			case 15:
				pullydown();
			break;
			case 63:
				pullyup();
			break;
			default:
				flag2=1;
				stop2();
			break;
		}
		if(flag1==1 && flag2==1)
		{
			bot_stop2();
			flag1=0;
			flag2=0;
		}
		
	}
	return 0;
}

