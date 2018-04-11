/*****************

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
int flag=0,check1=0,check2=0,temp1,temp2,x1,a1,a2,flags=0,flagr=0,flagh,flagb=0,i=0,step=0;
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
		//PWM('3','b',10000,f);
		SETBIT(PORTH,BIT(6));
		//SETBIT(PORTH,BIT(7));
		//PORTD |= (1<<PD5);
}

void leftclock(int r)
{
		flag=1;
		PWM('3','a',10000,r);
		//PWM('3','b',10000,r);
		//CLEARBIT(PORTH,BIT(7));
		CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}

void rightclock(int f)
{
		flag=1;
		PWM('3','b',10000,f);
		//PWM('3','b',10000,f);
		SETBIT(PORTH,BIT(7));
		//SETBIT(PORTH,BIT(7));
		//PORTD |= (1<<PD5);
}

void rightanti(int r)
{
		flag=1;
		PWM('3','b',10000,r);
		//PWM('3','b',10000,r);
		CLEARBIT(PORTH,BIT(7));
		//CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}


void BotUp(int r)
{
		flagb=1;
		PWM('3','c',10000,r);
		//PWM('3','b',10000,r);
		SETBIT(PORTG,BIT(3));
		//CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}

void BotDown(int r)
{
		flagb=1;
		PWM('3','c',10000,r);
		//PWM('3','b',10000,r);
		CLEARBIT(PORTG,BIT(3));
		//CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}

void shiftup(int r)
{
		flagb=1;
		PWM('4','a',10000,r);
		//PWM('3','b',10000,r);
		CLEARBIT(PORTG,BIT(4));
		//CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}

void shiftdown(int r)
{
		flagb=1;
		PWM('4','a',10000,r);
		//PWM('3','b',10000,r);
		SETBIT(PORTG,BIT(4));
		//CLEARBIT(PORTH,BIT(6));
		//PORTD |= (1<<PD5);

}

void stop1()
{
		PWM('3','a',10000,0);
		PWM('3','b',10000,0);
		PWM('3','c',10000,0);
		PWM('4','a',10000,0);
		//TOGGLEBIT(PORTH,BIT(6));
		//PWM('3','b',10000,0);
		//PWM('4','c',10000,0);
		//PWM('4','b',10000,0);
		if(flag==1)
		{
		//TOGGLEBIT(PORTH,BIT(7));
		//TOGGLEBIT(PORTH,BIT(6));
		//TOGGLEBIT(PORTL,bit(0));
		//TOGGLEBIT(PORTL,bit(1));
		flag=0;
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

void step_up()
{
	while(1)
	{
			while(i<4)
			{
			BotUp(100);
			while(PINL & (1<<PL3));
			while(!(PINL & (1<<PL3)));
			_delay_ms(50);
			while(!(PINL & (1<<PL3)));
			_delay_ms(1000);
			while(PINL & (1<<PL3));
			_delay_ms(700);
			BotDown(100);
			while(PINL & (1<<PL4));
			while(!(PINL & (1<<PL4)));
			_delay_ms(50);
			while(!(PINL & (1<<PL4)));
			_delay_ms(700);
			stop1();
			_delay_ms(1000);
			i++;
			}
			while(1);
	}

}

void see_saw()
{
		//while(1)
		//{
		a2=poll_adc();
		ADMUX++;
		//ADMUX++;
		//ADMUX++;
		a1=poll_adc();
		ADMUX--;
		//ADMUX--;
		//ADMUX--;
		//joystick_poll();
	
		if(flagh==1)
		{
			
			if(a1>960)
			{
				rightclock(40);
			}
			else if(a1<930)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=2;
				//_delay_ms(1000);
				
			}
		}
		if(flagh==2)
		{
			if(a2<970)
			{
				leftanti(40);
			}
			else if(a2>1000)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=1;
				//_delay_ms(2000);//inited
				//PORTA |= (1<<PA1);
				//_delay_ms(4000);
			}
		}
		/*if((PINC & (1<<PC4)))
		{
			break;
		}*/
	//	}

}
void retract()
{
	//while(1)
	//{
	
		a2=poll_adc();
		ADMUX++;
		//ADMUX++;
		//ADMUX++;
		a1=poll_adc();
		ADMUX--;
		//ADMUX--;
		//ADMUX--;
		//joystick_poll();
	
		if(flagh==1)
		{
			
			if(a1>640)
			{
				rightclock(40);
			}
			else if(a1<610)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=2;
				//_delay_ms(1000);
				
			}
		}
		if(flagh==2)
		{
			if(a2<650)
			{
				leftanti(40);
			}
			else if(a2>680)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=1;
				//_delay_ms(2000);//inited
				//PORTA |= (1<<PA1);
				//_delay_ms(4000);
			}
		}
		
		//if(!(PINC & (1<<PC4)))
		//{
		//	break;
		//}
	//}

}

void poll_walk()
{
	while(1)
	{
		a2=poll_adc();
		ADMUX++;
		//ADMUX++;
		//ADMUX++;
		a1=poll_adc();
		ADMUX--;
		//ADMUX--;
		//ADMUX--;
		//joystick_poll();
	
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
				flagh=2;
				_delay_ms(1000);
				
			}
		}
		if(flagh==2)
		{
			if(a2<760)
			{
				leftanti(40);
			}
			else if(a2>790)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=3;
				_delay_ms(2000);//inited
				PORTA |= (1<<PA4);
				_delay_ms(4000);
			}
		}
		
		if(flagh==3)
		{
			if(a2<1000)
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
				flagh=4;
				
			}
		}
		
		if(flagh==4)
		{
			
			if(a1>720)
			{
				rightclock(40);
			}
			else if(a1<700)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=5;
				_delay_ms(1000);
				PORTA |= (1<<PA6);
				_delay_ms(4000);//pole 2 grip
				PORTA &= ~(1<<PA4);
				
			}
		}
		
		if(flagh==5)
		{
			if(a1>330)
			{
				rightclock(40);
			}
			else if(a1<310)
			{
				rightanti(40);
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
			if(a2<360)
			{
				leftanti(40);
			}
			else if(a2>380)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=7;
				_delay_ms(1000);
				PORTA |= (1<<PA5);
				_delay_ms(3000);//pole 3
				PORTA &= ~(1<<PA6);
			}
		}
		if(flagh==7)
		{
			if(a2<600)
			{
				leftanti(40);
			}
			else if(a2>620)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=8;
				//_delay_ms(2000);
			}
		}
		if(flagh==8)
		{
			if(a1>990)
			{
				rightclock(40);
			}
			else if(a1<970)
			{
				rightanti(40);
			}
			else
			{
				lcd_gotoxy2(0);
				lcd_char('L');
				stop1();
				flagh=12;//pole 4 grab
				_delay_ms(1000);
				PORTA |= (1<<PA7);
				_delay_ms(3000);
				//PORTA &= ~(1<<PA5);
			}
		}
		
		if(flagh==9)
		{
			if(a1>370)
			{
				rightclock(40);
			}
			else if(a1<350)
			{
				rightanti(40);
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
			if(a2<300)
			{
				leftanti(40);
			}
			else if(a2>320)
			{
				leftclock(40);
			}
			else
			{
				lcd_gotoxy1(0);
				lcd_char('R');
				stop1();
				flagh=10;
				//_delay_ms(2000);
			}
			
		}
		
	}
		

}

void manual()
{
	
	while(1)
	{
			a2=poll_adc();
			ADMUX--;
			ADMUX--;
			ADMUX--;
			a1=poll_adc();
			ADMUX++;
			ADMUX++;
			ADMUX++;
			lcd_gotoxy1(0);
			lcd_showvalue(a2);
			lcd_gotoxy2(0);
			lcd_showvalue(a1);
			
		joystick_poll();
		switch(data0)
	   {
		case 136:
		leftclock(60);
		break;
		case 56:
		leftanti(60);
		break;
		case 120://left
		rightclock(60);
		break;
		case 24://right
		rightanti(60);
		break;
		case 224:
		shiftup(80);
		break;
		case 8:
		shiftdown(80);
		break;
		case 192://start
		BotUp(100);
		break;
		case 240:
		BotDown(100);
		break;
		default:
		stop1();
		break;
		}
		switch(data1)
		{
		case 195:
		PORTA &=~ (1<<PA6);
		break;
		case 240:
		PORTA |= (1<<PA6);
		break;
		case 225:
		PORTA &=~ (1<<PA5);
		break;
		case 199:
		PORTA |= (1<<PA5);
		break;
		case 143:
		PORTA |= (1<<PA7);
		break;
		case 63:
		PORTA &= ~(1<<PA7);
		break;
		case 31:
		PORTA |= (1<<PA4);
		break;
		case 127:
		PORTA &= ~(1<<PA4);
		break;
		default:
		//stop2();
		break;
		
		}
	
	
	}

}

int main() 
{
		
	int i=0;
		port_init();
		PWM_init('3','a');
		PWM_init('3','b');
		PWM_init('3','c');
		PWM_init('4','a');
		//PWM_init('3','c');
		//PWM_init('4','a');
		//PWM_init('4','b');
		//PWM_init('4','c');
		PWM('3','a',10000,0);
		PWM('3','b',10000,0);
		PWM('3','c',10000,0);
		PWM('4','a',10000,0);
		//PWM('3','c',10000,0);
		//PWM('4','a',10000,0);
		//PWM('4','b',10000,0);
		//PWM('4','c',10000,0);
		
		
		
		lcd_init();
		ps2_init();
		adc_init();
		//TCCR0B |= (1<<CS01) | (1<<CS02);
		//TCCR1B |= (1<<CS11) | (1<<CS12);
		DDRD &= ~(1<<PD6);
		PORTD |= (1<<PD6);
		DDRD &= ~(1<<PD7);
		PORTD |= (1<<PD7);
		DDRD |= (1<<PD5);
		PORTD |= (1<<PD5);
		//lcd_char('A');
  
			joystick_config();
			joystick_config();
			joystick_config();
			flagh=1;
			flags=1;
			flagr=1;
			i=0;
			ADMUX++;
			ADMUX++;
			step=1;

	while(1)
	{
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
					a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					if(step==1)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=2;
					}
					
					if(step==2)
					{
					while(a1>180)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=3;
					}
					if(step==3)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					_delay_ms(200);
					step=4;
					}
					if(step==4)
					{
						while(a2<190)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=5;
					//	stop1();
					}
					if(step==5)
					{
						while(a2>100)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=6;
						//stop1();
					}
				
					if(step==6)
					{
						while(a2<190)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
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
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=8;
					}
					
					if(step==8)
					{
					while(a1>180)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=9;
					}
					if(step==9)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					_delay_ms(200);
					step=10;
					}
					if(step==10)
					{
						while(a2<180)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=11;
					//	stop1();
					}
					if(step==11)
					{
						while(a2>100)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=12;
						//stop1();
					}
				
					if(step==12)
					{
						while(a2<180)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=13;
						_delay_ms(200);
						//stop1();
					}
					
					if(step==13)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=14;
					}
					
					if(step==14)
					{
					while(a1>180)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=15;
					}
					if(step==15)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					_delay_ms(200);
					step=16;
					}
					if(step==16)
					{
						while(a2<170)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=17;
					//	stop1();
					}
					if(step==17)
					{
						while(a2>100)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
					}
						step=18;
						//stop1();
					}
				
					if(step==18)
					{
						while(a2<170)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
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
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=20;
					}
					
					if(step==20)
					{
					while(a1>180)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=21;
					}
					if(step==21)
					{
					while(a1<480)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					_delay_ms(200);
					step=22;
					//stop1();
					}
					if(step==22)
					{
						BotUp(5);
						shiftdown(100);
						_delay_ms(4000);
						BotDown(100);
						_delay_ms(200);
						step=23;
						shiftup(100);
						_delay_ms(2500);
						stop1();
						//stop1();
						//BotDown(100);
						//_delay_ms(1000);
						//stop1();
						//step=23;
					
					}
				
					if(step==23)
					{
						while(a2>100)
					{
						a2=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2+=poll_adc();
						a2=a2/4;
						ADMUX--;
						ADMUX--;
						ADMUX--;
						a1=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1+=poll_adc();
						a1=a1/4;
						ADMUX++;
						ADMUX++;
						ADMUX++;
						BotDown(100);
						
					}
						step=24;
						//stop1();
					}
					
					if(step==24)
					{
					while(a1<300)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=25;
					}
					
					if(step==25)
					{
					while(a1<500)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					}
					step=26;
					}
					
					if(step==26)
					{
					while(a1>290)
					{
						a2=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2+=poll_adc();
					a2=a2/4;
					ADMUX--;
					ADMUX--;
					ADMUX--;
					a1=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1+=poll_adc();
					a1=a1/4;
					ADMUX++;
					ADMUX++;
					ADMUX++;
					BotUp(100);
					_delay_ms(200);
					}
					step=27;
					//stop1();
					}
					if(step==27)
					{
					BotUp(5);
					shiftdown(100);
					_delay_ms(9000);
					stop1();
					shiftup(100);
					_delay_ms(5000);
					stop1();
					BotDown(100);
					_delay_ms(2200);
					stop1();
					shiftdown(100);
					_delay_ms(3000);
					stop1();
					BotUp(100);
					_delay_ms(3000);
					stop1();
					shiftup(100);
					BotDown(100);
					_delay_ms(2000);
					stop1();
					step=28;
					_delay_ms(1000);
					PORTA |= (1<<PA2);
					_delay_ms(200);
					PORTA &= ~(1<<PA2);
					stop1();
					}
				//////////////////////////////////////////
					
					
				}
				/*
				a2=poll_adc();
				a2+=poll_adc();
				a2+=poll_adc();
				a2+=poll_adc();
				a2=a2/4;
				ADMUX--;
				ADMUX--;
				ADMUX--;
				a1=poll_adc();
				a1+=poll_adc();
				a1+=poll_adc();
				a1+=poll_adc();
				a1=a1/4;
				ADMUX++;
				ADMUX++;
				ADMUX++;
				if(step==1)
				{
					if(a1<300)
					{
					BotUp(100);
					}
					else
					{
					//_delay_ms(200);
					//stop1();
					step=2;
					}
				
				}
				if(step==2)
				{
					if(a1>120)
					{
					BotUp(100);
					}
					else
					{
					//_delay_ms(1000);
					//_delay_ms(200);
					//stop1();
					step=3;
					}
					
				}
				if(step==3)
				{
					if(a1<300)
					{
					BotUp(100);
					}
					else
					{
					_delay_ms(500);
					stop1();
					step=6;
					}
				
				}
				
				if(step==4)
				{
					if(a2<180)
					{
					BotDown(100);
					}
					else
					{
					_delay_ms(500);
					stop1();
					step=5;
					}
					
				}
				if(step==5)
				{
					if(a2<180)
					{
					BotDown(100);
					}
					else
					{
					_delay_ms(500);
					stop1();
					step=6;
					}
					
				}
				}
				
		}
		
		//lcd_gotoxy1(0);
		//lcd_showvalue(a2);
		//lcd_gotoxy2(0);
		//lcd_showvalue(a1);
		
		/*if(!(PINC & (1<<PC4)))
		{
			lcd_gotoxy1(0);
			lcd_char('R');
			retract();
		}
		else
		{
			lcd_gotoxy1(0);
			lcd_char('S');
			see_saw();
		
		}*/
		}
	}
	
	
}


