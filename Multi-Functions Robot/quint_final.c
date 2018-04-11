#include<avr/io.h>
#include<avr/interrupt.h>
#include"PORT.h"
#include"LCD.h"
#include<util/delay.h>
unsigned char count1=0,flag=0,stage=0,turn=0,count=0,stn=0,dtmf=0;


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

void trace_line(void)
{
        if(CHECKBIT(PIND,BIT(5)))//right
        {
                if(CHECKBIT(PIND,BIT(7)))//center
                        {
                                if(CHECKBIT(PIND,BIT(6)))//left
                                {
                                fwd(90);
                        
                                }
                                else
                                {
                 
                                right(90);
                                }
                        }
                else
                {
                if(CHECKBIT(PIND,BIT(6)))
                {
                        
                        left(120);
                        
                        
                }
           else
           {
                        right(120);
           }
         }
   }
   
   else
   {
     if(CHECKBIT(PIND,BIT(7)))
         {
           if(CHECKBIT(PIND,BIT(6)))
           {
            left(100);
           }
           else
           {   
                        
                fwd(130);
           }
         }
         else
         {
          if(CHECKBIT(PIND,BIT(6)))
           {
           left(100);
           }
           else
           {
                      //  if(stage==1)
                        //{
						 right(100);
                        //}
                       //else
                        //{ left(100);

                       //}
           }
         }
   }
}


void mobile()
{

        dtmf=PIND;
        dtmf=dtmf & 0x1b;
        if(dtmf==0x02)
        {
                fwd(150);
        }
        
        if(dtmf==0x10)
        {       
                
                back(150);
        }
        
        if(dtmf==0x08)
        {
                left(120);
        }
        
        if(dtmf==0x0A)
        {
                right(120);
        }
        
        if(dtmf==0x09)
        {
                stop();
        }
}


void manual()
{

        if(CHECKBIT(PINC,BIT(2)))
        {
                if(CHECKBIT(PINC,BIT(3))) 
                {
                        SETBIT(PORTC,BIT(0));
                        SETBIT(PORTC,BIT(1));
                }
                else
                {
                        SETBIT(PORTC,BIT(0));      
                        CLEARBIT(PORTC,BIT(1));   
                }

        }       
        else
        {
                if(CHECKBIT(PINC,BIT(3))) 
                {
                SETBIT(PORTC,BIT(1));
                CLEARBIT(PORTC,BIT(0));
                }
                else
                {
                        SETBIT(PORTC,BIT(0));
                        SETBIT(PORTC,BIT(1));
                }
        }

        if(CHECKBIT(PINC,BIT(4)))
        {
                if(CHECKBIT(PINC,BIT(5))) 
                {
                        SETBIT(PORTB,BIT(1));
                        SETBIT(PORTB,BIT(2));
                }
                else
                {
                        SETBIT(PORTB,BIT(1));      
                        CLEARBIT(PORTB,BIT(2));
        
                }

        }       
        else
        {
                if(CHECKBIT(PINC,BIT(5))) 
                {
                SETBIT(PORTB,BIT(2));
                CLEARBIT(PORTB,BIT(1));
                }
                else
                {
                        SETBIT(PORTB,BIT(2));
                        SETBIT(PORTB,BIT(1));
                }
        }


}




void obstacle()
{
        long unsigned int i,j;
        
        //i=1000;
        fwd(80);
        //void delay_ms(i);
        //right(100);
        //WaitMs(1000);
        //*void delay_ms(i);
        if(CHECKBIT(PIND,BIT(6)) || CHECKBIT(PIND,BIT(5)))
                {
                        //i=10000;
                        stop();
                //      WaitMs(1);
                    for(i=10000;i>0;i--)
                        {
                                delay_ns(25);
                        }
                        
                         for(i=1000;i>0;i--)
                        {
                                back(120);
                                delay_ns(7);
                        }
                        for(i=10000;i>0;i--)
                        {
                                delay_ns(25);
                        }
                    for(i=1000;i>0;i--)
                        {
                                right(100);
                                delay_ns(5);
                        }
                //      delay_ns(200);
                //  WaitMs(1);
                }

}

void edge_detector()
{

	/*

	if(!(CHECKBIT(PIND,BIT(6))))//right
	{
		if(!(CHECKBIT(PIND,BIT(7))))//center
			{
				if(!(CHECKBIT(PIND,BIT(5))))//left
				{


	*/

	long unsigned int i,j;
	
	fwd(80);
	
	if(!(CHECKBIT(PIND,BIT(7))))
		{
			
			stop();
		
		    for(i=10000;i>0;i--)
			{
				delay_ns(25);
			}
			
			 for(i=1000;i>0;i--)
			{
				back(120);
				delay_ns(7);
			}
			
		    for(i=1000;i>0;i--)
			{
				left(100);
				delay_ns(5);
			}
		
		}
	if(!(CHECKBIT(PIND,BIT(6)))) //&& CHECKBIT(PIND,BIT(7)) && CHECKBIT(PIND,BIT(6)))
		{
			
			stop();
		
		    for(i=10000;i>0;i--)
			{
				delay_ns(25);
			}
			
			 for(i=1000;i>0;i--)
			{
				back(120);
				delay_ns(7);
			}
			
		    for(i=1000;i>0;i--)
			{
				left(100);
				delay_ns(5);
			}
		
		}
	if(!(CHECKBIT(PIND,BIT(5)))) //&& CHECKBIT(PIND,BIT(7)) && CHECKBIT(PIND,BIT(6)))
		{
			
			stop();
		
		    for(i=10000;i>0;i--)
			{
				delay_ns(25);
			}
			
			 for(i=1000;i>0;i--)
			{
				back(120);
				delay_ns(7);
			}
			
		    for(i=1000;i>0;i--)
			{
				right(100);
				delay_ns(5);
			}
		
		}



}






int main(void)
{

ports_init();
        SETBIT(PORTC,BIT(4));
        SETBIT(PORTC,BIT(5));
        SETBIT(PORTB,BIT(6));
        SETBIT(PORTB,BIT(7));
        SETBIT(PORTB,BIT(0));
        SETBIT(PORTC,BIT(2));
        SETBIT(PORTC,BIT(3));
while(1)
{
        
        if(CHECKBIT(PINB,BIT(6)))
        {
                if(CHECKBIT(PINB,BIT(7)))
                {
                        if(CHECKBIT(PINB,BIT(0)))
                        {
                                trace_line();
                        }
                        else
                        {
                                manual();
                        }
                }
                else
                {
                        if(CHECKBIT(PINB,BIT(0)))
                        {
                                obstacle();
                        }
                        else
                        {
                                mobile();
                        }
                }
        }
        else
        {
                if(CHECKBIT(PINB,BIT(7)))
                {
                        if(CHECKBIT(PINB,BIT(0)))
                        {
                                edge_detector();
                        }
                }
                
        }
}
}
