/*****************

pot1 702
pot2 643
PORTD pins 0-4 Joystick
PWM1-OC3A- Motor 1
PWM2-OC3B Motor 2
pneumatics PJ0 side 1
PJ1 side 2
ADC side 1 ADC1 PF1
side 2 ADC 3 PF3

changelog: 0.1  Date 28/10/13
Req:
PWM 3a,3b,3c,4a for base motors
4b for Disc motor
Need 3 more motors
T0 counter for disc
T1 for encoder X
T5 for encoder Y
And i guess we have to multiplex it as we are short on PWM channels
One port for Joystick

changelog 0.2
Time 20:00
Joystick interface done with 640

changelog 0.3
Time 23:11
Code ready for Disc and base motors using Joystick
What left is Encoders Multiplexing and Hardcoding
Todays work is done. Tomorrow only plug and play. (Still space for 4 motors left...Use wisely)
Timers 1 and 5 left T0 will go for 8bit timer and input is from disc wala  PD7
As timer 1 is PD6 we can make use of same FRC thus PD6 that is PD6 will be our Base encoder counter and it will be Muxed. 

changelog 0.4
Time 12:31

changelog 0.5
time12:10
positioning 2 : 20-224 fwrd is anti rev is clock fwrd extreme is 224 rev extreme is 20
positioning 1 : 227-46	right is anti left is clock right extreme is 227 left extreme is 46

changelog 0.6
Time 16:32
POT values identified
POT 1 seems to be faulty
POT 2 totally fine
POT 1-- from 0 - 180  
POT 2-- from 0 - 180  830-143

changelog 0.7 
time 2:22
values noted

519
255

449
143

768
785

197
255



left press to 284 right press to 86
up to 861 down to 152

changelog 9.25:

initA1 =480 if ( 572  do 56 230 do 136)
A2 = 554 if(192 do 24 527 do 120)

changelog 1:46

Pole walk done


******************/

#include <avr/io.h>
#include <avr/delay.h>
#include <ctype.h>
#include "PWM640.h"
#include "LCD640.h"
#include "joystick.h" 
int flag=0,check1=0,check2=0,temp1,temp2,x1,a1,a2,flagse=0,flagre=0,flagh,flagb=0,flage=1,flagf=0,i=0,step=0,data=0,swing1=0;
uint16_t tempc;



void PWM_init(char ch,char sub)
{

	setPWM(ch,sub,10000);

}


void port_init()
{
	DDRE=0xFF;
	DDRH=0xFF;
	DDRL=0xFF;
	DDRG=0xFF;
	DDRJ=0xFF;
	PORTJ = 0x00;
	DDRF=0x00;
	DDRA=0xFF;
	PORTA=0x00;
	DDRL=0x00;
	PORTL=0xFF;
	DDRB=0x00;
	PORTB=0xFF;
	DDRC=0x00;
	PORTC=0xFF;
}

void leftanti(int f)
{
		flag=1;
		PWM('3','a',10000,f);
		
		SETBIT(PORTH,BIT(6));
		
}

void leftclock(int r)
{
		flag=1;
		PWM('3','a',10000,r);
		
		
		CLEARBIT(PORTH,BIT(6));
		

}

void rightclock(int f)
{
		flag=1;
		PWM('3','b',10000,f);
		
		SETBIT(PORTH,BIT(7));
		
}

void rightanti(int r)
{
		flag=1;
		PWM('3','b',10000,r);
		
		CLEARBIT(PORTH,BIT(7));
		

}


void BotUp(int r)
{
		flagb=1;
		PWM('3','c',10000,r);
		
		SETBIT(PORTG,BIT(3));
		

}

void BotDown(int r)
{
		flagb=1;
		PWM('3','c',10000,r);
		
		CLEARBIT(PORTG,BIT(3));
		

}

void shiftup(int r)
{
		flagb=1;
		PWM('4','a',10000,r);
		
		CLEARBIT(PORTG,BIT(4));
		

}

void shiftdown(int r)
{
		flagb=1;
		PWM('4','a',10000,r);
		
		SETBIT(PORTG,BIT(4));
		

}

void stop1()
{
		PWM('3','a',10000,0);
		PWM('3','b',10000,0);
		PWM('3','c',10000,0);
		PWM('4','a',10000,0);
		
		if(flag==1)
		{
			flag=0;
		}
		if(flagb==1)
		{
		
			flagb=0;
		}

}

void stop2()
{
		PWM('3','a',10000,0);
		PWM('3','b',10000,0);
		PWM('3','c',10000,0);
		PWM('4','a',10000,0);
		
		if(flag==1)
		{
		TOGGLEBIT(PORTH,BIT(7));
		TOGGLEBIT(PORTH,BIT(6));
		
		flag=0;
		}
		if(flagb==1)
		{
			TOGGLEBIT(PORTG,BIT(3));
			TOGGLEBIT(PORTG,BIT(4));
			flagb=0;
		}

}


void adc_init()
{
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= (1<<MUX0);

}

int poll_adc()
{
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	x1= ADCL;
	temp=ADCH;
	temp=temp<<8;
	temp=temp | x1;
	_delay_us(40);
	return temp;
}

void fold()
{
		
		a2=poll_adc();
		ADMUX |= (1<<MUX1);
		ADMUX &= ~(1<<MUX0);
		
		a1=poll_adc();
		ADMUX |= (1<<MUX0);
		ADMUX &= ~(1<<MUX1);
		
	
		if(flagf==1)
		{
			
			if(a1>340)
			{
				rightclock(40);
			}
			
			else
			{
				stop1();
				flagf=2;
				
				
			}
		}
		if(flagf==2)
		{
			if(a2<330)
			{
				leftanti(40);
			}
			
			else
			{
				
				stop1();
				flagf=3;
				
			}
		}
		

}

void see_saw()
{
		
		a2=poll_adc();
		ADMUX |= (1<<MUX1);
		ADMUX &= ~(1<<MUX0);
		
		a1=poll_adc();
		ADMUX |= (1<<MUX0);
		ADMUX &= ~(1<<MUX1);
		
	
		if(flagse==1)
		{
			
						if(a1<890)
			{
				rightanti(70);
			}
			else
			{
				stop1();
				flagse=2;
		
				
			}
		}
		if(flagse==2)
		{
			if(a2<880)
			{
				leftanti(70);
			}
		
			else
			{
				stop1();
				flagse=3;
				
			}
		}
		
		

}
void retract()
{
		
		a2=poll_adc();
		ADMUX |= (1<<MUX1);
		ADMUX &= ~(1<<MUX0);
		
		a1=poll_adc();
		ADMUX |= (1<<MUX0);
		ADMUX &= ~(1<<MUX1);
		
		reswing();
		
		
		if(flagre==1)
		{
			
			if(a1>320)
			{
				rightclock(70);
			}
			
			else
			{
				stop1();
				flagre=2;
				
			}
		}
		if(flagre==2)
		{
			
			if(a2>330)
			{
				leftclock(70);
			}
			else
			{
				stop1();
				flagre=3;
				
			}
		}
		

}


void poll_walk()
{
		a2=poll_adc();
		ADMUX |= (1<<MUX1);
		ADMUX &= ~(1<<MUX0);
		//ADMUX++;
		//ADMUX++;
		a1=poll_adc();
		ADMUX |= (1<<MUX0);
		ADMUX &= ~(1<<MUX1);
		//ADMUX--;
		//ADMUX--;
	
		if(flagh==1)
		{
			
			if(a1>650)
			{
				rightclock(40);
			}
			else if(a1<630)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=3;
				
				
			}
		}
		if(flagh==2)
		{
			if(a2<650)
			{
				leftanti(40);
			}
			else if(a2>670)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=3;
				_delay_ms(1000);//inited
				PORTJ |= (1<<PJ5);
				_delay_ms(1000);
			}
		}
		
		if(flagh==3)
		{
			if(a2<1015)
			{
				leftanti(40);
			}
			else if(a2>1020)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				PORTJ |= (1<<PJ6);
				_delay_ms(1000);//pole 2 grip
				PORTJ &= ~(1<<PJ5);
				flagh=5;
				
			}
		}
		
		if(flagh==4)
		{
			
			if(a1>660)
			{
				rightclock(40);
			}
			else if(a1<650)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=5;
				
				PORTJ |= (1<<PJ6);
				_delay_ms(1000);//pole 2 grip
				PORTJ &= ~(1<<PJ5);
				
			}
		}
		
		if(flagh==5)
		{
			if(a1>330)
			{
				rightclock(50);
			}
			else if(a1<320)
			{
				rightanti(50);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=6;
			}
		}
		if(flagh==6)
		{
			if(a2<330)
			{
				leftanti(50);
			}
			else if(a2>350)
			{
				leftclock(50);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				
				PORTJ |= (1<<PJ7);
				_delay_ms(1000);//pole 3
				PORTJ &= ~(1<<PJ6);
				flagh=7;
			}
		}
		if(flagh==7)
		{
			if(a2<640)
			{
				leftanti(40);
			}
			else if(a2>670)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=8;
				
			}
		}
		if(flagh==8)
		{
			if(a1>980)
			{
				rightclock(50);
			}
			else if(a1<970)
			{
				rightanti(50);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=9;//pole 4 grab
				_delay_ms(500);
				PORTJ |= (1<<PJ4);
				_delay_ms(1000);
				PORTJ &= ~(1<<PJ7);
				
			}
		}
		
		if(flagh==9)
		{
			if(a1>470)
			{
				rightclock(70);
			}
			else if(a1<450)
			{
				rightanti(70);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=10;//wait for manual
			}
			
		}
		
		if(flagh==10)
		{
			if(a2<500)
			{
				leftanti(70);
			}
			else if(a2>520)
			{
				leftclock(70);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=11;
				
			}
		
			
		}
		
		

}

void distance()
{
					a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX &= ~(1<<MUX0);
					ADMUX &= ~(1<<MUX1);
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX |= (1<<MUX0);
					ADMUX |= (1<<MUX1);	

}

void ladder()
{
					distance();
					if(step==1)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					step=2;
					}
					
					if(step==2)
					{
					while(a1>180)
					{
					distance();
					BotUp(100);
					}
					step=3;
					}
					if(step==3)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					_delay_ms(200);
					step=4;
					}
					if(step==4)
					{
						while(a2<190)
					{
						distance();
						BotDown(100);
					}
						step=5;
					
					}
					if(step==5)
					{
						while(a2>100)
					{
						distance();
						BotDown(100);
					}
						step=6;
					
					}
				
					if(step==6)
					{
						while(a2<190)
					{
						distance();
						BotDown(100);
					}
						step=7;
						_delay_ms(200);
						stop1();
					}
					
					if(step==7)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					step=8;
					}
					
					if(step==8)
					{
					while(a1>180)
					{
					distance();
					BotUp(100);
					}
					step=9;
					}
					if(step==9)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					_delay_ms(200);
					step=10;
					}
					if(step==10)
					{
						while(a2<180)
					{
						distance();
						BotDown(100);
					}
						step=11;
										}
					if(step==11)
					{
						while(a2>100)
					{
						distance();
						BotDown(100);
					}
						step=12;
					
					}
				
					if(step==12)
					{
						while(a2<180)
					{
						distance();
						BotDown(100);
					}
						step=13;
						_delay_ms(200);
					
					}
					
					if(step==13)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					step=14;
					}
					
					if(step==14)
					{
					while(a1>180)
					{
					distance();
					BotUp(100);
					}
					step=15;
					}
					if(step==15)
					{
					while(a1<300)
					{
					distance();
					BotUp(100);
					}
					_delay_ms(200);
					step=16;
					}
					if(step==16)
					{
						while(a2<170)
					{
						distance();
						BotDown(100);
					}
						step=17;
					
					}
					if(step==17)
					{
						while(a2>100)
					{
						distance();
						BotDown(100);
					}
						step=18;
					
					}
				
					if(step==18)
					{
						while(a2<170)
					{
						distance();
						BotDown(100);
					}
						step=19;
						_delay_ms(200);
						stop1();
					}
					
					if(step==19)
					{
					while(a1<300)
					{
						distance();
					BotUp(100);
					}
					step=20;
					}
					
					if(step==20)
					{
					while(a1>180)
					{
						distance();
					BotUp(100);
					}
					step=21;
					}
					if(step==21)
					{
					while(a1<480)
					{
					distance();
					BotUp(100);
					}
					_delay_ms(200);
					step=22;
					
					}
					if(step==22)
					{
						BotUp(5);
						shiftdown(100);
						_delay_ms(2400);
						BotDown(100);
						_delay_ms(200);
						step=23;
						shiftup(100);
						_delay_ms(1500);
						stop1();
					
					}
					if(step==23)
					{
						while(a2>100)
					{
						distance();
						BotDown(100);
					}
						step=24;
					
					}
					if(step==24)
					{
					while(a1<300)
					{
						distance();
						BotUp(100);
					}
						step=25;
					
					}
				
					if(step==25)
					{
					while(a1<500)
					{
						distance();
						BotUp(100);
					}
					step=26;
					}
					if(step==26)
					{
					while(a1>290)
					{
						distance();
						BotUp(100);
						_delay_ms(200);
					}
					step=27;
					
					}
					if(step==27)
					{
					BotUp(5);
					shiftdown(100);
					_delay_ms(2000);
					stop1();
					shiftup(100);
					_delay_ms(1000);
					stop1();
					BotDown(100);
					_delay_ms(1500);
					stop1();
					shiftdown(100);
					_delay_ms(1000);
					stop1();
					BotUp(100);
					_delay_ms(1000);
					stop1();
					shiftup(100);
					BotDown(100);
					_delay_ms(1000);
					stop1();
					step=28;
					_delay_ms(1000);
					PORTJ |= (1<<PJ3);
					_delay_ms(200);
					PORTJ &= ~(1<<PJ3);
					stop1();
					}
				
					
						
					
}


void manual()
{
	
	while(1)
	{
			a2=poll_adc();
		ADMUX |= (1<<MUX1);
		ADMUX &= ~(1<<MUX0);
		
		a1=poll_adc();
		ADMUX |= (1<<MUX0);
		ADMUX &= ~(1<<MUX1);
			lcd_gotoxy1(0);
			lcd_showvalue(a2);
			lcd_gotoxy2(0);
			lcd_showvalue(a1);
			if(!(PINB & (1<<PB7)))
			{
				if(!(PINB & (1<<PB7)))
				{
					if(!(PINB & (1<<PB7)))
					{
					lcd_gotoxy2(8);
					lcd_showvalue(0);
					}
				}
			}
		else
			{
				lcd_gotoxy2(8);
				lcd_showvalue(1);
			}
			
		joystick_poll();
		switch(data0)
	   {
		case 0:
		leftclock(60);
		break;
		case 48:
		leftanti(60);
		break;
		case 112://left
		rightclock(60);
		break;
		case 16://right
		rightanti(60);
		break;
		case 192:
		shiftup(80);
		break;
		case 128://start
		BotUp(100);
		break;
		case 224:
		BotDown(100);
		break;
		default:
		stop1();
		break;
		}
		switch(data1)
		{
		case 135:
		PORTJ &=~ (1<<PJ6);
		break;
		case 293:
		PORTJ |= (1<<PJ6);
		break;
		case 224:
		PORTJ &=~ (1<<PJ5);
		break;
		case 195:
		PORTJ |= (1<<PJ5);
		break;
		case 15:
		PORTJ |= (1<<PJ7);
		break;
		case 63:
		PORTJ &= ~(1<<PJ7);
		break;
		case 31:
		PORTJ |= (1<<PJ4);
		break;
		case 127:
		PORTJ &= ~(1<<PJ4);
		break;
		default:
		
		break;
		
		}
	
	
	
	}

}

void swing()
{
	
		if(swing1==0)
		{
		
			BotUp(100);
			
			_delay_ms(1200);
			
			stop1();
		
		swing1=1;
		_delay_ms(2000);
		}
		else
		{
		
		}

}
void reswing()
{
	PORTJ &= ~(1<<PJ5);
		if(swing1==1)
		{
			BotDown(100);
			_delay_ms(900);
			stop1();
		
		
		swing1=0;
		
		}
		else
		{
		
		}
}

int main() 
{
		
	int i=0,task=0;
		port_init();
		PORTB &= ~(1<<PB7);
		PWM_init('3','a');
		PWM_init('3','b');
		PWM_init('3','c');
		PWM_init('4','a');
		PWM('3','a',10000,0);
		PWM('3','b',10000,0);
		PWM('3','c',10000,0);
		PWM('4','a',10000,0);
				
		lcd_init();
		ps2_init();
		adc_init();
		DDRD &= ~(1<<PD6);
		PORTD |= (1<<PD6);
		DDRD &= ~(1<<PD7);
		PORTD |= (1<<PD7);
		DDRD |= (1<<PD5);
		PORTD |= (1<<PD5);
		
  
			joystick_config();
			joystick_config();
			joystick_config();
			flagh=1;
			flagse=1;
			flagre=1;
			flagf=1;
			flage=0;
			
			i=0;
			ADMUX++;
			ADMUX++;
			step=3;
			
			
			
			if(!(PINB & (1<<PB5)))
			{
						lcd_gotoxy1(0);
						lcd_string("Joystick");
						manual();
					
			}
			else
			{
		
		while(1)
		{

				
		
			if((PINC & (1<<PC4)))
			{
				if(flagre==3)
				{
					flagre=0;
				
				}
				if(flagse==3)
				{
					flagse=0;
				
				}
				if(flagf==3)
				{
					flagf=0;
				
				}
		
				switch(task)
				{
					case 0:
						
						lcd_gotoxy1(0);
						lcd_string("Retract");
					break;
					case 1:
						see_saw();
						lcd_gotoxy1(0);
						lcd_string("Seesaw ");
					
					break;
					case 2:
						poll_walk();
						lcd_gotoxy1(0);
						lcd_string("Pole   ");
						
					break;
					case 3:
						
						lcd_gotoxy1(0);
						lcd_string("swing  ");
						swing();
						
					break;
					case 4:
						_delay_ms(2000);
						ladder();
						lcd_gotoxy1(0);
						lcd_string("Ladder ");
						
					break;
					case 5:
						lcd_gotoxy1(0);
						lcd_string("Nothing");
						stop1();
					
					break;
					default:
						lcd_gotoxy1(0);
						lcd_string("Nothing");
						stop1();
					
					break;
				
				}
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_string("Give input");
				if(flagre==0)
				{
					flagre=1;
				}
				if(flagf==0)
				{
					flagf=1;
				}
				if(flagse==0)
				{
					flagse=1;
				}
				stop1();
				data= PINC & 0x0F;
				
				
				switch(data)
				{
					case 0b00001000:
						task=1;
						lcd_gotoxy2(0);
						lcd_string("None ");
					break;
					case 0b00000100:
						task=2;

						a2=poll_adc();
						ADMUX |= (1<<MUX1);
						ADMUX &= ~(1<<MUX0);
						
						a1=poll_adc();
						ADMUX |= (1<<MUX0);
						ADMUX &= ~(1<<MUX1);
						
					
					if(a1<510)
					{
						rightanti(40);
					}
					else
					{
						lcd_gotoxy2(0);
						lcd_char('L');
						stop1();
					}
					PORTJ |= (1<<PJ5);
					lcd_gotoxy2(0);
					lcd_string("Grab ");					
					flagse=3;
						
					break;
					case 0b00000010:
						task=3;
						retract();
						lcd_gotoxy2(0);
						lcd_string("Folds");
						
					break;
					case 0b00000001:
						task=4;
						retract();
						lcd_gotoxy2(0);
						lcd_string("Foldl");
						BotUp(5);
						
					break;
					case 0b00000000:
						task=0;
						
						PORTJ &= ~(1<<PJ5);
						retract();
						retract();
						
					break;
					case 0b00001111:
						task=5;
						break;
				
				}
			
			}
			}
	}

	
}