#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;



void port_init()
{
	SETBIT(DDRC,BIT(0));
	SETBIT(DDRC,BIT(1));
	CLEARBIT(DDRC,BIT(2));
	CLEARBIT(DDRC,BIT(3));
	CLEARBIT(DDRC,BIT(5));
	CLEARBIT(DDRC,BIT(4));
	DDRB=0XFF;
	
}

void adc_init()
{
	ADMUX=0b01100010;
	ADCSRA=0b10001011;

}

int poll_adc()
{
	ADCSRA |= (1<<ADSC);               //SETBIT(ADCSRA,BIT(6))
	while(!(ADCSRA & (1<<ADIF)));        //while(!CHECKBIT(ADCSRA,BIT(4)))
	return ADCH;

}

void delay_ns(unsigned char tim)
{
	int l,n;
        for(l=0;l<tim;l++)
        {
                for(n=0;n<10;n++)
                {};
        }
}


void fwd(int speed)
{
        SETBIT(PORTC,BIT(0));      
        CLEARBIT(PORTC,BIT(1));
        SETBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(200-speed);        
}


void back(int speed)
{
        SETBIT(PORTC,BIT(1));      
        CLEARBIT(PORTC,BIT(0));
        SETBIT(PORTB,BIT(2));
        CLEARBIT(PORTB,BIT(1));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(200-speed);
}



void right(int speed)
{
        SETBIT(PORTC,BIT(0));      
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(200-speed);  
}

void left(int speed)
{
        CLEARBIT(PORTC,BIT(0));      
        CLEARBIT(PORTC,BIT(1));
        SETBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        delay_ns(200-speed);	
}

void dance(int speed)
{
        CLEARBIT(PORTC,BIT(0));      
        SETBIT(PORTC,BIT(1));
        SETBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
        _delay_ms(1500);           
        SETBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        SETBIT(PORTB,BIT(2));
        _delay_ms(1500);	
}

void stop(void)
{
        CLEARBIT(PORTC,BIT(0));   
        CLEARBIT(PORTC,BIT(1));
        CLEARBIT(PORTB,BIT(1));
        CLEARBIT(PORTB,BIT(2));
}


int main() 
{
	int tempx,tempy,tempz;

		port_init();
		//lcd_init();
		adc_init();
		DDRB=0XFF;
		TCNT1=0;
		TCCR1A=0XAA;
		TCCR1B=0X12;
	
	
		SETBIT(PINC,BIT(5));
		SETBIT(PINC,BIT(4));
        	SETBIT(PINC,BIT(0));
		SETBIT(PINC,BIT(1));
		SETBIT(PINB,BIT(2));
		SETBIT(PINB,BIT(1));
		
 	while(1)
	{
		

		tempx=poll_adc();
		ADMUX++;
		tempy=poll_adc();
		ADMUX++;
		tempz=poll_adc();	
		ADMUX--;
		ADMUX--;
		//lcd_gotoxy1(3);
		//lcd_showvalue(tempx);
		//lcd_gotoxy1(9);
		//lcd_showvalue(tempy);
	//lcd_gotoxy2(0);
		//lcd_showvalue(tempz);

		
	
		
		
			if(tempz>90)
			{
			//PORTD=0b00000001;//back
			//back(200);
			ICR1=1200;
			OCR1A=600;
			}
			else if(tempz<70)
			{
			//PORTD=0b00000010;//fwd
			
			//OCR1A=0;
			ICR1=1500;
			OCR1A=750;
			}
			else if(tempx>90)
			{
			//PORTD=0b00000100;//right
			//right(150);
			ICR1=1950;
			OCR1A=975;
			}
			else if(tempx<70)
			{
			//PORTD=0b00001000;//left
			//left(150);
			ICR1=1700;
			OCR1A=850;
			}
			else if(!(CHECKBIT(PINC,BIT(4))))
			{
			//PORTD=0b00000000;//STOP
			//stop();
			}
			else if((!(CHECKBIT(PINC,BIT(4)))) && (!(CHECKBIT(PINC,BIT(5)))))
			{
			//PORTD=0b00000000;//STOP
			//dance(200);
			}
			else
			{
				//PORTD=0b00000000;//STOP
				ICR1=0;
			OCR1A=0;
			//	_delay_ms(100);
			}
			
		
	
	}
}

