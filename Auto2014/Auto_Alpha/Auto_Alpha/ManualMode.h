#include "joystick.h"
void Manual_Mode(void)
{
	
	while(1)
	{
		joystick_poll();
		lcd_cmd(LINE1);
		lcd_showvalue(data0);
		lcd_gotoxy2(0);
		lcd_showvalue(GetADCvalue(1));		//left pot
		lcd_gotoxy2(8);
		lcd_showvalue(GetADCvalue(2));		//RIGHT POT
		switch(data0)
		{
			case 0:
			Left_Motor_ClockWise(500);
			break;
			case 48:
			Left_Motor_AntiClockWise(500);
			break;
			case 112://left
			Right_Motor_ClockWise(500);
			break;
			case 16://right
			Right_Motor_AntiClockWise(500);
			break;
			
			case 128://start
			Rail_Up();
			break;
			case 224:
			Rail_Down();
			break;
			default:
			All_Motors_stop();
			break;
		}
		switch(data1)
		{
			case 135:
			PORTJ &=~ (1<<PJ6);
			break;
			case 293:
			PORTJ |= (1<<PJ6);
			break;
			case 224:
			PORTJ &=~ (1<<PJ5);
			break;
			case 195:
			PORTJ |= (1<<PJ5);
			break;
			case 15:
			PORTJ |= (1<<PJ7);
			break;
			case 63:
			PORTJ &= ~(1<<PJ7);
			break;
			case 31:
			PORTJ |= (1<<PJ4);
			break;
			case 127:
			PORTJ &= ~(1<<PJ4);
			break;
			default:
			
			break;
		}
	}
}