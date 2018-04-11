/*
 * Functions.h
 *
 * Created: 09-02-2014 20:05:36
 *  Author: Coder Rajat
 */ 

/*
void DiscStop(void)
{
	PWM('4','A',10000,0,TOGGLE);
}

void DiscRight(uint8_t s)
{
	PWM('4','A',10000,s,SET);
}

void DiscLeft(uint8_t s)
{
	PWM('4','A',10000,s,CLEAR);
}

void LeftFwd(int speed)
{
	SETBIT(PORTJ,BIT(0));
	CLEARBIT(PORTJ,BIT(1));
	//PWM('3','A',10000,speed,CLEAR);
	OCR3A = speed * 7.37;
}

void LeftRev(int speed)
{
	SETBIT(PORTJ,BIT(1));
	CLEARBIT(PORTJ,BIT(0));
	//PWM('3','A',10000,speed,CLEAR);
	OCR3A = speed * 7.37;
}

void LeftStop(void)
{
	SETBIT(PORTJ,BIT(0));
	CLEARBIT(PORTJ,BIT(1));
	//PWM('3','A',10000,20,CLEAR);
	OCR3A = 145;
}


void RightFwd(int speed)
{
	SETBIT(PORTJ,BIT(3));
	CLEARBIT(PORTJ,BIT(2));
	//PWM('3','B',10000,speed,CLEAR);
	OCR3B = speed * 7.37;
}

void RightRev(int speed)
{
	SETBIT(PORTJ,BIT(2));
	CLEARBIT(PORTJ,BIT(3));
	//PWM('3','B',10000,speed,CLEAR);
	OCR3B = speed * 7.37;
}

void RightStop(void)
{	
	SETBIT(PORTJ,BIT(3));
	CLEARBIT(PORTJ,BIT(2));
	//PWM('3','B',10000,20,CLEAR);
	OCR3B = 145;
}


void MainPullyStop(void)
{
	PWM('4','B',10000,10,CLEAR);
}

void MainPullyUp(void)
{
	PWM('4','B',10000,100,CLEAR);
}

void MainPullyDown(void)
{
	PWM('4','B',10000,100,SET);
}


void Pully2Stop(void)
{
	PWM('5','A',10000,2,CLEAR);
}

void Pully2Up(void)
{
	PWM('5','A',10000,100,CLEAR);
}

void Pully2Down(void)
{
	PWM('5','A',10000,100,SET);
}

void ExecuteSeeSaw(void)
{
	PORTA |= (1<<PA3);
	for(int i = 0; i < 3; i++)
	{
		PORTA |= (1<<PA1);
		_delay_ms(1000);
		PORTA &= ~(1<<PA1);
		_delay_ms(1000);
	}
	PORTA &= ~(1<<PA3);
	PORTA &= ~(1<<PA1);
	
}*/