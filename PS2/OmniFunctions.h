#include <math.h>
#define true 1
#define false 0
int Speed[4];
#define Enc_Diameter 100
#define Enc_ppr 1000
uint8_t Stop[4] = {true, true, true, true};



uint32_t Get_Distance(int x,int y)
{
	x -= 132;								
	y -= 132;
	
	if (x == -132 && y == -132)
	{
		return	187;
	} 
	else
	{
		return (sqrt((x*x) + (y*y)));
	}
	
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

void OmniDrive3Wheel(float Velocity, int Angle, int Rotation)
{
	Speed[0] = ceil(Velocity*sin((60 + Angle)*3.142857/180) + Rotation);
	Speed[1] = ceil(Velocity*sin((Angle)*3.142857/180) - Rotation);
	Speed[2] = ceil(Velocity*sin((60 - Angle)*3.142857/180) - Rotation);
	
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
	
	OCR3A = Speed[0];			//back left
	OCR3B = Speed[1];			//front
	OCR4A = Speed[2];			//back right
}


void Omni_Enc_Drive(int Angle, uint64_t Dist)		//int mm
{
	int  X_ppr = 0, Adjustment = 0;
	double Y_ppr = 0;
	float Ratio,C_Ratio;
	int Ap, Am;
	switch (Angle)
	{
		case 0:	case 180:
		Y_ppr = (Enc_ppr*Dist)/Enc_Diameter;
		CLEARBIT(DDRE,BIT(4));
		while (Enc_Y() <= Y_ppr)
		{
			OmniDrive3Wheel(2000, Angle,Adjustment);		
			if (Enc_X()>cde)
			{
				Adjustment -= abc;
				if (Enc_X()>cde*4)
				{
					ResetEnc_X();
					Adjustment = 0;
				}
				/*if (Direction_X() == 1)
				{
					Adjustment +=20;
					if (Direction_X() == 2)
					{
						Adjustment = 0;
						ResetEnc_X();
					}
				}
				else if (Direction_X() == 2)
				{
					Adjustment -=20;
					if (Direction_X() == 1)
					{
						Adjustment = 0;
						ResetEnc_X();
					}
				}*/
			}
		}
		/*
		check the rotation parameter(Adjustment)
		check for Directions
		Adjust the value of Adjustment parameter accordingly
		*/
		SETBIT(DDRE,BIT(4));
		break;
		
		case 90: case -90:
		break;
		
		default:
			Ratio = (sin(Angle))/(cos(Angle));
			Y_ppr = (Enc_ppr*(Dist*cos(Angle)))/Enc_Diameter;
			lcd_gotoxy1(8);
			lcd_showvaluef(Y_ppr);
			//lcd_showvalue(Y_ppr%100);
			while (Enc_Y() < Y_ppr)
			{
				OmniDrive3Wheel(1000, Angle,0);
				C_Ratio = Enc_X()/Enc_Y();
				if (C_Ratio > Ratio)
				{
					if (Am < cde)
					{
						Am++;
					}
					else 
					{
						Am = 0;
						Angle--;
					}
				}
				else if(C_Ratio < Ratio)
				{
					if (Ap < cde)
					{
						Ap++;
					}
					else
					{
						Ap = 0;
						Angle--;
					}
				}
				else
				{
					Am = 0;
					Ap = 0;
				}
			}
				
		break;
	}
	OmniStop();
}

/*
	
							(Left set)
								|	front
								|
								|
								|
								|
						      /	  \
						     /	   \
							/	    \
						   /		 \
		(fwrd set)		  /			  \		(fwrd set)
					back left		  back right
*/


/*
						|(123,0)
		+ve angle		|		-ve angle
						|
			2.			|		1.
						|
  (0,123)				|					(255,123)
	  __________________|___________________
						|(123,123)
						|
						|
						|
			3.			|		4.
						|
						|(123,255)

				*/
