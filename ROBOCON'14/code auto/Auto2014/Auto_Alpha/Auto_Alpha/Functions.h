#define LeftNormal 375
#define RightNormal 410
/*------------------Init Functions-------------------*/
void PortInit(void)
{
	DDRE |= (1<<PE3) | (1<<PE4) | (1<<PE5);
	DDRH |= (1<<PH3) | (1<<PH6) | (1<<PH7);
	DDRG |= (1<<PG3) | (1<<PG4);
	DDRF = 0x00;
	DDRC = 0x00;
	SETBIT(PORTC, BIT(4));
	DDRJ = 0xFF;
	PORTJ = 0x00;
}

void PwmInit(void)
{
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
}
/*--------------------------------------------------*/

/*------------------ADC Functions-------------------*/
void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1<<ADSC);	
}
int GetADCvalue(int x)
{
	if (x == 0)
	{
		CLEARBIT(ADMUX, BIT(0));
		CLEARBIT(ADMUX, BIT(1));
	}
	else if (x == 1)
	{
		SETBIT(ADMUX, BIT(0));
		CLEARBIT(ADMUX, BIT(1));
	}
	else if (x == 2)
	{
		SETBIT(ADMUX, BIT(1));
		CLEARBIT(ADMUX, BIT(0));
	}
	else if (x == 3)
	{
		SETBIT(ADMUX, BIT(1));
		SETBIT(ADMUX, BIT(0));
	}
	ADCSRA |= (1<<ADSC);	
	while( !(ADCSRA & (1<<ADIF)));
	return ADC;
}

int GetSharpInput(int x)
{
	return ((GetADCvalue(x)+GetADCvalue(x)+GetADCvalue(x)+GetADCvalue(x))/4);
}
/*--------------------------------------------------*/


/*------------------Rail Functions-------------------*/
void Rail_Up(void)
{
	SETBIT(PORTG, BIT(3));
	OCR3C = 737;
}

void Rail_Down(void)
{
	CLEARBIT(PORTG, BIT(3));
	OCR3C = 737;
}

void Rail_Stop(void)
{
	TOGGLEBIT(PORTG, BIT(3));
	OCR3C = 0;
}
/*--------------------------------------------------*/

/*------------------Plane Functions-------------------*/
void PlaneUp(void)
{
	SETBIT(PORTG, BIT(4));
	OCR4A = 737;
}

void PlaneDown(void)
{
	CLEARBIT(PORTG, BIT(4));
	OCR4A = 737;
}

void PlaneStop(void)
{
	TOGGLEBIT(PORTG, BIT(4));
	OCR4A = 0;
}
/*--------------------------------------------------*/

/*-------------Left Motor Functions-----------------*/

//ADC 1
void Left_Motor_ClockWise(int speed)
{
	SETBIT(PORTH, BIT(6));
	OCR3A = speed;
}

void Left_Motor_AntiClockWise(int speed)
{
	CLEARBIT(PORTH, BIT(6));
	OCR3A = speed;
}

void Left_Motor_Stop(void)
{
	TOGGLEBIT(PORTH, BIT(6));
	OCR3A = 0;
}
/*--------------------------------------------------*/

/*-------------Right Motor Functions-----------------*/

//ADC 2
void Right_Motor_ClockWise(int speed)
{
	SETBIT(PORTH, BIT(7));
	OCR3B = speed;
}

void Right_Motor_AntiClockWise(int speed)
{
	CLEARBIT(PORTH, BIT(7));
	OCR3B = speed;
}

void Right_Motor_Stop(void)
{
	TOGGLEBIT(PORTH, BIT(7));
	OCR3B = 0;
}
/*--------------------------------------------------*/

void All_Motors_stop(void)
{
	Rail_Stop();
	Left_Motor_Stop();
	Right_Motor_Stop();
}

void All_Motors_Normal_Position(void)
{
	while (GetADCvalue(1) <= (LeftNormal -10))
	{
		Left_Motor_ClockWise(300);
		//lcd_gotoxy1(0);
		//lcd_showvalue(GetADCvalue(1));
	}
	All_Motors_stop();
	while (GetADCvalue(1) >= (LeftNormal + 10))
	{
		Left_Motor_AntiClockWise(300);
		//lcd_gotoxy1(0);
		//lcd_showvalue(GetADCvalue(1));
	}
	All_Motors_stop();
	while (GetADCvalue(2) <= RightNormal)
	{
		Right_Motor_AntiClockWise(300);
		//lcd_cmd(LINE2);
		//lcd_showvalue(GetADCvalue(2));
	}
	All_Motors_stop();
	while (GetADCvalue(2) >= RightNormal + 10)
	{
		Right_Motor_ClockWise(300);
		//lcd_cmd(LINE2);
		//lcd_showvalue(GetADCvalue(2));
	}
	All_Motors_stop();
}

