/*
 * Auto_Alpha.c
 *
 * Created: 3/22/2014 10:55:42 PM
 *  Author: YateenKedare
 * LOGS	:	Plain Shifter Connections Faulty
 * Done: SeeSaw
 * Done: Swing(just adjust the pick up time)
 */ 


#include <avr/io.h>
#include <util/delay.h>
# define F_CPU 14745600
#include "R_LCD640.h"
#include "Functions.h"
//#include "ManualMode.h"
#include "SeeSaw.h"
#include "Swing.h"
#include "PoleWalk.h"
#include "JungleGym.h"

int main(void)
{
	int Task = 20;
	uint8_t Pole_Walk_Flag = 0;
	lcd_init();
	PortInit();
	PwmInit();
	ADC_Init();
	
	//ps2_init();
	//
	//joystick_config();
	//joystick_config();
	//joystick_config();
	//
	//
	//while (1)
	//{
		//All_Motors_Normal_Position();
		//_delay_ms(1000);
		//PoleWalk_Adjust_Initial_Value();
		//_delay_ms(500);
		//PoleWalk();
		//_delay_ms(5000);
		//break;
	//}
	//
	//if(bit_is_clear(PINB, 5))
	//{
		//lcd_gotoxy1(0);
		//lcd_string("Joystick");
		//Manual_Mode();
	//}
	//else
	//{	
		
		//while (1)
		{
			//All_Motors_Normal_Position();
			//Rail_Up();
			//_delay_ms(1200);
			Rail_Down();
			_delay_ms(2500);
			All_Motors_stop();
			break;
		}
		
		while(1)
		{	//break;
			if (bit_is_clear(PINC, 4))
			{
				if (Task == 20)
				{
					All_Motors_Normal_Position();
					Task = 50;
				}
				
				if (Task == 21)
				{
					SwingRailRetract();
					Task = 50;
				}
				
				if (Task == 0)
				{
					if (Pole_Walk_Flag == 1)
					{
						SETBIT(PORTJ, BIT(5));
						Pole_Walk_Flag = 2;				//any random value
					}
					else if (Pole_Walk_Flag == 3)
					{
						CLEARBIT(PORTJ, BIT(4));
						_delay_ms(1000);
						All_Motors_Normal_Position();
						Pole_Walk_Flag = 0;
					}
				}
				
				Task = PINC & 0x0F;
				
				if (Task == 2)
				{
					if (Pole_Walk_Flag == 0)
					{
						PoleWalk_Adjust_Initial_Value();
						Pole_Walk_Flag = 1;
					}
					
				}
							
								
				lcd_gotoxy2(0);
				lcd_showvalue(Task);
			}
			else
			{
				_delay_ms(2000);
				switch(Task)
				
				{
					case 1:
					lcd_gotoxy1(0);
					lcd_string("SeeSa");
					SeeSaw();
					Task = 20;
					break;
					
					case 2:
					lcd_gotoxy1(0);
					lcd_string("PoleW");
					PoleWalk();
					Pole_Walk_Flag = 3;
					Task = 80;
					break;
					
					case 4:
					lcd_gotoxy1(0);
					lcd_string("Swing");
					Swing();
					Task = 21;				//some random value so that it dose not go into case 4 again and when gripped SwingRetract Gets Called
					break;
					
					//case 8:
					//lcd_gotoxy1(0);
					//lcd_string("Ladde");
					//JungleGym();
						//ladder
					break;
					default:
					break;
				}					
			}
		}
	//}		
}