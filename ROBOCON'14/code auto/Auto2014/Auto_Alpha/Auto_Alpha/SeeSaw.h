/*
LEFT MOTOR ADC 1
RIGHT MOTOR ADC 2
*/

#define LeftDown 890
#define RightDown	880

void SeeSaw(void)
{
	while (GetADCvalue(1) <  LeftDown)
	{
		Left_Motor_ClockWise(500);
		//lcd_gotoxy1(0);
		//lcd_showvalue(GetADCvalue(1));
	}
	
	All_Motors_stop();
	
	while (GetADCvalue(2) < RightDown)
	{
		Right_Motor_AntiClockWise(500);
		//lcd_cmd(LINE2);
		//lcd_showvalue(GetADCvalue(2));

	}
	All_Motors_stop();
}