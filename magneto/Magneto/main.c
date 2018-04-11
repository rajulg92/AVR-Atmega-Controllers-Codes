//PB0-1-2 Limit switches
//PC0,1,2- XYZ ANalog
//PD0,1,2,3 Transmit
//ATMEGA 8 TX

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"PORT.h"
unsigned char count1=0,flag=0,stage=0,turn=0,count=0,stn=0,dtmf=0,i=0;j=0,k=0;


void delay_ns(unsigned char tim)
{
        for(j=0;j<tim;j++)
        {
                for(k=0;k<10;k++)
                {};
        }
}


void fwd(int speed)  // generate pwm for forward motion of bot, 'speed' decides the duty cycle of that pwm
{
                      //010 stage is considered as 0 stage
        stage=0;
        SETBIT(PORTC,BIT(0));      
        CLEARBIT(PORTC,BIT(1));
        SETBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(speed);           //gives the ON time  
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(200-speed);        //gives the OFF time
}


void back(int speed)
{
        stage=0;
        SETBIT(PORTC,BIT(1));
        CLEARBIT(PORTC,BIT(0));
        SETBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(speed);
        CLEARBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(200-speed);
}



void left(int speed)
{
        stage=1;
        SETBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        delay_ns(speed);
        CLEARBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(200-speed);

}

void right(int speed)
{       stage=2;
        SETBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(speed);
        CLEARBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(200-speed);
        
}

void stop(void)
{
        CLEARBIT(PORTC,BIT(0));
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
}

void port_init()
{
	DDRC=0X00;
	DDRB=0X06;
	DDRD=0XFF;
}

void adc_init()
{
	ADMUX=0b01100010;
	ADCSRA=0b10000011;

}

int poll_adc()
{
	ADCSRA |= (1<<ADSC);               //SETBIT(ADCSRA,BIT(6))
	while(!(ADCSRA & (1<<ADIF)));        //while(!CHECKBIT(ADCSRA,BIT(4)))
	return ADCH;

}

int main() 
{
	int tempx,tempy,tempz;

		port_init();
		adc_init();
		
        
		
 	while(1)
	{
		tempy=poll_adc();
		ADMUX--;
		tempx=poll_adc();
		ADMUX--;
		tempz=poll_adc();
		
		ADMUX++;
		ADMUX++;
		
	
		
		
			if(tempy>90)
			{
			PORTD=0b00000001;//back
			//back(150);
			}
			else if(tempy<70)
			{
			PORTD=0b00000010;//fwd
			//fwd(150);
			}
			else if(tempx>90)
			{
			PORTD=0b00000100;//right
			//right(150);
			}
			else if(tempx<70)
			{
			PORTD=0b00001000;//left
			//left(150);
			}
			//else if(!(CHECKBIT(PORTC,BIT(4))))
			//{
			//PORTD=0b00000000;//STOP
			//stop();
			//}
			else
			{
				PORTD=0b00000000;//STOP
				//stop();
				_delay_ms(100);
			}
			
		
	
	}
}