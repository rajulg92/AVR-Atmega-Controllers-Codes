#include<avr/io.h>
#include<avr/interrupt.h>
#include"PORT.h"
#include"LCD.h"
//#include<delay.h>
unsigned char rencflag=0,lencflag=0,axis=0,d1speed,d2speed,d3speed,d4speed,p0flag=0,p1flag=0,p2flag=0,p3flag=0;
unsigned int renccnt=0,lenccnt=0,p1lcnt=0,p2lcnt=0,p3lcnt=0,p0lcnt=0;
unsigned int temp=0,stage=0;
void startrenccnt()
{
	
	if(renccnt!=0)
	{
		if(CHECKBIT(PIND,BIT(5)))
		{
			if(rencflag==0)
			{
				rencflag=1;
				renccnt--;
			}
					
		}
		else
		{
			rencflag=0;
		}
	}

}



void startlenccnt()
{
	
	if(lenccnt!=0)
	{
		if(CHECKBIT(PIND,BIT(6)))
		{
			if(lencflag==0)
			{
				lencflag=1;
				lenccnt--;
			}
					
		}
		else
		{
			lencflag=0;
		}
	}

}

void interrupt_init()
{

	SETBIT(SREG,BIT(7));

}


void pwm_init()
{
	TCCR1A=0XA8;
	TCNT1=0;
	TCNT3=0;
	TCCR1B=0X12;
	TCCR3A=0XA8;
	TCCR3B=0X12;
	ICR3=13824;
	OCR3A=0;
	OCR3B=0;
	OCR3C=0;
	ICR1=13824;
	OCR1A=0;
	OCR1B=0;
	OCR1C=0;	

	TCCR2=0X0A;
	TCNT2=0;
	OCR2=43;
	SETBIT(TIMSK,BIT(7));

	
}

int motorspeed(unsigned char flspeed,unsigned char frspeed,unsigned char blspeed,unsigned char brspeed)
{
	{
		temp=~(flspeed);
		CLEARBIT(temp,BIT(6));
		CLEARBIT(temp,BIT(7));
		PORTF=temp;
		_delay_us(100);
	}
	{
		temp=~(frspeed);
		CLEARBIT(temp,BIT(6));
		SETBIT(temp,BIT(7));
		PORTF=temp;
		_delay_us(100);
		
	}
	{
		temp=~(blspeed);
		SETBIT(temp,BIT(6));
		CLEARBIT(temp,BIT(7));
		PORTF=temp;
		_delay_us(100);
		
	}
	{
		temp=~(brspeed);
		SETBIT(temp,BIT(6));
		SETBIT(temp,BIT(7));
		PORTF=temp;
		_delay_us(100);

	}

}



TIMER2_OVF_vect()
{
			//startlenccnt();
		//lcd_cmd(LINE1);
		//lcd_showvalue(temp++);
}



void motorforward()
{
	CLEARBIT(PORTG,BIT(3));
	CLEARBIT(PORTE,BIT(2));
	CLEARBIT(PORTB,BIT(0));
	CLEARBIT(PORTB,BIT(3));

}


void motorbackward()
{
	SETBIT(PORTG,BIT(3));
	SETBIT(PORTE,BIT(2));
	SETBIT(PORTB,BIT(0));
	SETBIT(PORTB,BIT(3));

}



void servofront()
{
	//motorspeed(0,0,0,0);
	//_delay_ms(10);
	axis=0;
	OCR3A=1163;
	OCR3B=1160;
	OCR3C=1190;
	OCR1C=1340;		// += clockwise,    - = anti
	_delay_ms(10);


}



void servoside()
{	
	axis=1;
	//motorspeed(0,0,0,0);
	//_delay_ms(10);

	OCR3A=525;
	OCR3B=525;
	OCR3C=532;
	OCR1C=695;
	_delay_ms(10);


}

void servo45()
{	
	axis=1;
	//motorspeed(0,0,0,0);
	//_delay_ms(10);

	OCR3A=850;
	OCR3B=819;
	OCR3C=532;
	OCR1C=695;
	_delay_ms(10);
}	


void startlinecnt()
{
	if(axis==0)
	{
		if(CHECKBIT(PIND,BIT(1)))
			{
				if(p0flag==0)
				{
					p0flag=1;
				}
			}
		else
			{
				if(p0flag)
				{
					p0lcnt++;
					p0flag=0;
				}
			}

		if(CHECKBIT(PIND,BIT(2)))
			{
				if(p1flag==0)
				{
					p1flag=1;
				}
			}
		else
			{
				if(p1flag)
				{
					p1lcnt++;
					p1flag=0;
				}
			}
	}


	if(axis==1)
	{

	if(CHECKBIT(PIND,BIT(0)))
			{
				if(p2flag==0)
				{
					p2flag=1;
				}
			}
		else
			{
				if(p2flag)
				{
					p2lcnt++;
					p2flag=0;
				}
			}
		if(CHECKBIT(PIND,BIT(3)))
			{
				if(p3flag==0)
				{
					p3flag=1;
				}
			}
		else
			{
				if(p3flag)
				{
					p3lcnt++;
					p3flag=0;
				}
			}
	}

}



int main()
{
	port_init();
//interrupt_init();

	pwm_init();

//servofront();
servo45();



motorforward();
	//PORTA=0X88;
lcd_init();
lcd_cmd(LINE1);
lcd_string("shri ganesh");

d1speed=d2speed=63;
d3speed=d4speed=63;
motorspeed(0,0,0,0);	// (OCR0,OCR2,OCR1B,OCR1A) (D4,D3,D1,D2 )
	// (OCR0,OCR2,OCR1B,OCR1A)
	//lenccnt=200;
	for(;;)
	{
	
motorspeed(d1speed,d2speed,d3speed,d4speed);


	if(stage==1)
	{
		lcd_cmd(LINE1);
		lcd_showvalue(p2lcnt);
		
		startlinecnt();
		
		if(p2lcnt==4 || p1lcnt==4 )
		{
		
			d1speed=d2speed=d3speed=d4speed=20;


		}
		if(p2lcnt==5 ||p1lcnt==5 )
		{
		
			d1speed=d2speed=d3speed=d4speed=20;


		}





		if(p2lcnt==6 || p1lcnt==6)
		{
			d1speed=d2speed=d3speed=d4speed=15;
		}
		
					
		
		if(p1lcnt==7 || p2lcnt==7)
		{

			d1speed=d2speed=15;
			d3speed=d4speed=15;
		
			stage=2;
			p1lcnt=p2lcnt=p3lcnt=p0lcnt=0;
			lenccnt=110;
		}

	
		
		}		//stage 1 ends
	
	
	
	if(stage==2) // stage 2 starts	
		{
			
			startlenccnt();
			//lcd_cmd(LINE1);
			//lcd_showvalue(lenccnt);

			if(lenccnt==0)
			{
				motorbackward();
				d1speed=d2speed=0;
				d3speed=d4speed=0;
				_delay_ms(50);
				stage=3;

			}
						
		}
	if(stage==3)
		{
				motorforward();
				d1speed=d2speed=10;
				d3speed=d4speed=10;

				if(!(CHECKBIT(PING,BIT(0))))
				{
					motorbackward();
					d1speed=d2speed=0;
					d3speed=d4speed=0;
					p1lcnt=p2lcnt=p3lcnt=p0lcnt=0;
					_delay_ms(500);
					motorbackward();
					d1speed=d2speed=10;
					d3speed=d4speed=10;
					stage=5;
  				}
			    
		}		
							p1lcnt=p2lcnt=p3lcnt=p0lcnt=0;

	if(stage==5)
		{
			startlinecnt();
			lcd_cmd(LINE1);
			//lcd_showvalue(p1lcnt);
			lcd_string("hi");
			if(p1lcnt>=1 || p2lcnt>=1)
			{
				d3speed=d4speed=0;	
				d1speed=d2speed=0;	

			}
		}

	}
	return 0;
}
