/*
 * LEDCube.c
 *
 * Created: 5/27/2014 5:00:58 PM
 *  Author: YateenKedare
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include "Functions.h"
#include "Patterns.h"
int main(void)
{
	int r = 0;
	Cube_init();
	Test_all_LEDs();
	r = 0;	
   while(1)
    {
		switch (r)
		{
			case 0:
				for (int i = 0; i <9; i ++)
				{
					Rectangle_loop();
				}
			break;
			case 1:	Fall();
			break;
			case 2:	PlaneShift();
			break;
			case 3:	Glow_Random();
			break;
			case 4:
				for (int i= 0; i < 5; i++)
				{
					FillCube();
				}
			break;
			case 5:
				for (int i= 0; i < 10; i++)
				{
					OneByOneLEDOn();
				}
			break;
			case 6:
				for (int i= 0; i < 9; i++)
				{
					RandomFall();
				}
			break;
			case 7:
				for (int i = 0; i < 9;i++)
				{
					Vret_Rect_loop();
				}
			break;
			case 8:	Plane_flip();
			break;
			case 9:
				for (int i = 0; i < 10;i++)
				{
					InOut();
				}
			break;
				
		}

		r++;
		if(r>10)	r = 0;
    }
}

	//8 9 7 1 3 5 6 4 2