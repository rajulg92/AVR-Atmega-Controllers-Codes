/*****************
changelog: 0.1  Date 28/10/13
Req:
PWM 3a,3b,3c,4a for base motors
4b for Disc motor
Need 3 more motors
T0 counter for disc
T1 for encoder X
T5 for encoder Y
And i guess we have to multiplex it as we are short on PWM channels
One port for Joystick

changelog 0.2
Time 20:00
Joystick interface done with 640

changelog 0.3
Time 23:11
Code ready for Disc and base motors using Joystick
What left is Encoders Multiplexing and Hardcoding
Todays work is done. Tomorrow only plug and play. (Still space for 4 motors left...Use wisely)
Timers 1 and 5 left T0 will go for 8bit timer and input is from disc wala  PD7
As timer 1 is PD6 we can make use of same FRC thus PD6 that is PD6 will be our Base encoder counter and it will be Muxed. 

changelog 0.4
Time 12:31



******************/

#include <avr/io.h>
#include <avr/delay.h>
#include <ctype.h>
#include "PWM640.h"
#include "LCD640.h"
#include "joystick.h" 

void main()
{

	lcd_init();
	ps2_init();
  
      joystick_config();
	  joystick_config();
	  joystick_config();//more than once Required to Put it in RED mode If called once Analog is to be set

while(1)
{
		joystick_poll();
       
		lcd_gotoxy2(0);
		lcd_showvalueh(data0);
		lcd_gotoxy2(8);
		lcd_showvalueh(data1);
		lcd_gotoxy1(0);
		lcd_showvalueh(data2);
		lcd_gotoxy1(4);
		lcd_showvalueh(data3);
		lcd_gotoxy1(9);
		lcd_showvalueh(data4);
		lcd_gotoxy1(13);
		lcd_showvalueh(data5);

}
}

