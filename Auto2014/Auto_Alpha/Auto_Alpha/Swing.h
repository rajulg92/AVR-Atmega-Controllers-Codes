#define LeftUP	390
#define RightUP	395
void Swing()
{
	Rail_Up();
	_delay_ms(1100);
	Rail_Stop();
	while(GetADCvalue(1) > LeftUP)
	{
		Left_Motor_AntiClockWise(500);
	}
	All_Motors_stop();
	while (GetADCvalue(2) > RightUP)
	{
		Right_Motor_ClockWise(500);
	}
	All_Motors_stop();
}

void SwingRailRetract(void)
{
	_delay_ms(2000);				//pick up delay
	Rail_Down();
	_delay_ms(1000);
	Rail_Stop();
}