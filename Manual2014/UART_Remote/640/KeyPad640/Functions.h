/*
 * Functions.h
 *
 * Created: 09-02-2014 20:05:36
 *  Author: Coder Rajat
 */ 


void DiscStop()
{
	PWM('4','A',0,0,TOGGLE);
}

void DiscRight()
{
	PWM('4','A',10000,100,SET);
}

void DiscLeft()
{
	PWM('4','A',10000,100,CLEAR);
}

void LeftFwd(int speed)
{
	SETBIT(PORTJ,BIT(0));
	CLEARBIT(PORTJ,BIT(1));
	PWM('3','A',10000,speed,CLEAR);
}

void LeftRev(int speed)
{
	SETBIT(PORTJ,BIT(1));
	CLEARBIT(PORTJ,BIT(0));
	PWM('3','A',10000,speed,CLEAR);
}

void LeftStop()
{
	SETBIT(PORTJ,BIT(1));
	CLEARBIT(PORTJ,BIT(0));
	PWM('3','A',10000,20,CLEAR);
}


void RightFwd(int speed)
{
	SETBIT(PORTJ,BIT(2));
	CLEARBIT(PORTJ,BIT(3));
	PWM('3','B',10000,speed,CLEAR);
}

void RightRev(int speed)
{
	SETBIT(PORTJ,BIT(3));
	CLEARBIT(PORTJ,BIT(2));
	PWM('3','B',10000,speed,CLEAR);
}

void RightStop()
{	
	SETBIT(PORTJ,BIT(3));
	CLEARBIT(PORTJ,BIT(2));
	PWM('3','B',10000,20,CLEAR);
}


void MainPullyStop()
{
	PWM('4','B',0,0,TOGGLE);
}

void MainPullyUp()
{
	PWM('4','B',10000,100,SET);
}

void MainPullyDown()
{
	PWM('4','B',10000,100,CLEAR);
}


void Pully2Stop()
{
	PWM('1','A',0,0,TOGGLE);
}

void Pully2Up()
{
	PWM('1','A',10000,100,SET);
}

void Pully2Down()
{
	PWM('1','A',10000,100,CLEAR);
}