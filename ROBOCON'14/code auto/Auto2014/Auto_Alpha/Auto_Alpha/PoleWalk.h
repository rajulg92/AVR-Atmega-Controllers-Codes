/*
Pneumatics
BOTTOM RIGHT	PJ5
BOTTOM LEFT		PJ4
TOP LEFT		PJ6
TOP RIGHT		PJ7
*/



#define RightInitialValue 510


void PoleWalk_Adjust_Initial_Value(void)
{
	while (GetADCvalue(2) > 650)
	{
		Right_Motor_ClockWise(300);
	}
	while (GetADCvalue(2) < 630)
	{
		Right_Motor_AntiClockWise(300);
	}
	All_Motors_stop();
}


void PoleWalk(void)
{
	SETBIT(PORTJ, BIT(5));
	_delay_ms(1000);
	
	
	while (GetADCvalue(1) < 1015)
	{
		Left_Motor_ClockWise(300);
	}
	while (GetADCvalue(1) > 1020)
	{
		Left_Motor_AntiClockWise(300);
	}
	All_Motors_stop();
	_delay_ms(1000);
	/*_____Grab Pole 2 And leave Pole 1_____*/
	
	SETBIT(PORTJ, BIT(6));
	_delay_ms(1500);
	CLEARBIT(PORTJ, BIT(5));
	_delay_ms(500);
	
	/*______________________________________*/
	
	while (GetADCvalue(2) > 330)
	{
		Right_Motor_ClockWise(400);
	}
	while (GetADCvalue(2) < 320)
	{
		Right_Motor_AntiClockWise(400);
	}
	All_Motors_stop();
	
	while (GetADCvalue(1) <	340)
	{
		Left_Motor_ClockWise(400);
	}
	
	while (GetADCvalue(1) > 350)
	{
		Left_Motor_AntiClockWise(400);
	}
	All_Motors_stop();
	_delay_ms(1000);
	/*_____Grab Pole 3 And leave Pole 2_____*/
	
	SETBIT(PORTJ, BIT(1));
	SETBIT(PORTJ, BIT(1));
	_delay_ms(1500);
	CLEARBIT(PORTJ, BIT(6));
	_delay_ms(500);
	
	/*______________________________________*/
	
	while (GetADCvalue(1) < 640)
	{
		Left_Motor_ClockWise(300);
	}
	while (GetADCvalue(1) > 670)
	{
		Left_Motor_AntiClockWise(300);
	}
	All_Motors_stop();
	
	while (GetADCvalue(2) > 980)
	{
		Right_Motor_ClockWise(400);
		lcd_gotoxy1(0);
		lcd_showvalue(GetADCvalue(2));
	}
	while (GetADCvalue(2) < 958)
	{
		Right_Motor_AntiClockWise(400);
		lcd_gotoxy2(0);
		lcd_showvalue(GetADCvalue(2));
	}
	All_Motors_stop();
	_delay_ms(1000);
	/*_____Grab Pole 4 And leave Pole 3_____*/
	
	SETBIT(PORTJ, BIT(4));
	_delay_ms(1000);
	CLEARBIT(PORTJ, BIT(1));
	_delay_ms(50);
	
	/*______________________________________*/
	
	
	
	while (GetADCvalue(1) < 500)
	{
		Left_Motor_ClockWise(530);
	}
	while (GetADCvalue(1) >520)
	{
		Left_Motor_AntiClockWise(530);
	}
	All_Motors_stop();
	
	while (GetADCvalue(2) > 470)
	{
		Right_Motor_ClockWise(530);
	}
	while (GetADCvalue(2) < 450)
	{
		Right_Motor_AntiClockWise(530);
	}
	All_Motors_stop();
}