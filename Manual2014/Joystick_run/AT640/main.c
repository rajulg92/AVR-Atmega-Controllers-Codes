#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "LCD_640.h"
#include "joystick.h" 
#include <stdbool.h>
int i_data1 = 0, i_check = 0;
void trace_circular_curve()
{
	while(1)
	{
		PORTL |= (1<<0) | (1<<1);
		OCR4B = 737;
		OCR4C = 646;
	}
}
void pwm_init()
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
}

void port_init()
{
	DDRH |= (1 << PH4) | (1 << PH5);		//OC3B	//OC3A	DIRECTION BITS
	DDRL |= (1<<PL0)   | (1<<PL1);	//OC3A, 3B O/P DECLARATION
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
void bot_stop()
{
	OCR4B = 0;
	OCR4C = 0;
	PORTL ^= _BV(0) ^ _BV(1);
}

void bot_stop2()
{
	OCR4B = 0;
	OCR4C = 0;
}

typedef enum _bit
{
	CLEAR = 0, SET = 1
}_bit;

int main(void)
{
	lcd_init();
	
	unsigned int x;
	enum _bit y = 0;
	lcd_showvalue(sizeof(y));
	
	//lcd_string("  ");
	//lcd_showvaluef(sqrt((x*x)+(y*y)));
	return 0;
	
	
	ps2_init();
	port_init();
	pwm_init();

	joystick_config();
	joystick_config();
	joystick_config();
	
	while(1)
	{
		joystick_poll();
		
		/*lcd_gotoxy2(0);
		lcd_showvalue(data1);
		lcd_gotoxy2(8);
		lcd_showvalue(data0);
		lcd_gotoxy1(0);
		*/switch(data1)
	    {
			case 63:	//63
				leftREV(700);
				rightREV(700);
				i_check = 'r';
	//			lcd_string("REVERSE");
			break;
			case 15:	//15
				leftFWD(700);
				rightFWD(700);
				i_check = 'f';
	//			lcd_string("FORWARD");
			break;
			case 31:	//31
				if(i_check == 'f')
				{
					leftFWD(700);
					rightFWD(30);
				}
				else if(i_check == 'r')
				{
					leftREV(700);
					rightREV(30);
				}
	//			lcd_string("RIGHT");
			break;	
			case 127:	//127
				if(i_check == 'f')
				{
					leftFWD(30);
					rightFWD(700);	
				}
				else if(i_check == 'r')
				{
					leftREV(30);
					rightREV(700);
				}
	//			lcd_string("LEFT");
			break;
			case 131:
			case 195:
				leftFWD(400);
				rightFWD(700);
			break;
			
			case 135:
				leftFWD(700);
				rightFWD(400);
			break;
			case 224:
				bot_stop2();
	//			lcd_string("STOP");
			break;
			case 192:
				bot_stop2();
	//			lcd_string("STOP");
			break;
			case 240:
				bot_stop2();
	//			lcd_string("STOP");
			break;
			default:
				if(i_data1 ==63 || i_data1 ==15 || i_data1 == 31 || i_data1 == 127 || i_data1 == 131 || i_data1 == 195 || i_data1 == 135)
					bot_stop();
				else
					bot_stop2();
	//			lcd_string("STOP");
			break;
		}
		i_data1 = data1;
		data1 = 255;
	}
	return 0;
}

