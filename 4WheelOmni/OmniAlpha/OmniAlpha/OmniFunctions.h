#include <math.h>
#define true 1
#define false 0
int Speed[4];
uint8_t Stop[4] = {true, true, true, true};
/*
int Get_Distance(int x,int y)
{
	x -= 132;
	y -= 132;
	if ((x == 0) && (y== 0))
	{
		return 0;
	} 
	else
	{
		return 1000;
	}
	
}
*/


int Get_Distance(int x,int y)
{
	x -= 132;								
	y -= 132;
	return (sqrt(x*x + y*y))*5;
}


int Get_Angle(int x, float y)
{
	x -= 132;
	y -= 132;
	if(x == 0 && y <=0)
	{
		return 0;
	}
	else if(x == 0 && y > 0)
	{
		return 180;
	}
	else if(x > 0)
		return -(ceil(atan((y)/(x))*180/3.1428) + 90);
	else if(x < 0)
		return -(ceil(atan((y)/(x))*180/3.1428) - 90);
	return 0;
}

void OmniStop(void)
{
	TOGGLEBIT(PORTH, BIT(6));
	TOGGLEBIT(PORTG, BIT(4));
	TOGGLEBIT(PORTH, BIT(7));
	TOGGLEBIT(PORTL, BIT(0));	
	OCR3A = 0;
	OCR3B = 0;
	OCR4A = 0;
	OCR4B = 0;
}

void OmniDrive(int Velocity, int Angle, int Rotation)
{
	Speed[0] = ceil(Velocity*sin((45 - Angle)*3.142857/180) + Rotation);
	Speed[1] = ceil(Velocity*sin((45 + Angle)*3.142857/180) - Rotation);
	Speed[2] = ceil(Velocity*sin((45 - Angle)*3.142857/180) - Rotation);
	Speed[3] = ceil(Velocity*sin((45 + Angle)*3.142857/180) + Rotation);
	
	if (Speed[0] == 0)
	{
		if (Stop[0] == true)
		{
			TOGGLEBIT(PORTH, BIT(6));						//for braking
			Stop[0] = false;
		}
		
	}
	else if (Speed[0] > 0)
	{
		SETBIT(PORTH, BIT(6));
		Stop[0] = true;
	}
	else
	{
		CLEARBIT(PORTH, BIT(6));
		Speed[0] = -Speed[0];
		Stop[0] = true;
	}
	
	if (Speed[1] == 0)
	{
		if (Stop[1] == true)
		{
			TOGGLEBIT(PORTH, BIT(7));
			Stop[1] = false;
		}
		
	}
	else if (Speed[1] > 0)
	{
		SETBIT(PORTH, BIT(7));
		Stop[1] = true;
	}
	else
	{
		CLEARBIT(PORTH, BIT(7));
		Speed[1] = -Speed[1];
		Stop[1] = true;
	}
	
	if (Speed[2] == 0)
	{
		if (Stop[2] == true)
		{
			TOGGLEBIT(PORTG, BIT(4));
			Stop[2] = false;
		}
		
	}
	else if (Speed[2] > 0)
	{
		SETBIT(PORTG, BIT(4));
		Stop[2] = true;
	}
	else
	{
		CLEARBIT(PORTG, BIT(4));
		Speed[2] = -Speed[2];
		Stop[2] = true;
	}
	
	if (Speed[3] == 0)
	{
		if (Stop[3] == true)
		{
			TOGGLEBIT(PORTL, BIT(0));
			Stop[4] = false;
		}
	}
	else if (Speed[3] > 0)
	{
		SETBIT(PORTL, BIT(0));
		Stop[4] = true;
	}
	else
	{
		CLEARBIT(PORTL, BIT(0));
		Speed[3] = -Speed[3];
		Stop[4] = true;
	}
	
	OCR3A = Speed[0];			//front left
	OCR3B = Speed[1];			//front right
	OCR4A = Speed[2];			//back right
	OCR4B = Speed[3];			//bach left
}


