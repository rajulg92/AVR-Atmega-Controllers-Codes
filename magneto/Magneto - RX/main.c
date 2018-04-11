//PC2,3,4,5 -motors
//PD0,1,2,3-recieved bits
//ATMEGA 16 RX

#include <avr/io.h>
#include <util/delay.h>
#include "PORT.h"

int l,n;

void port_init()
{
	DDRA=0xFF;                  //PORT DECLARED AS OUPTPUT PORT
	DDRB=0xFF;
	DDRC=0XFF;
	DDRD=0x00;                  //PORT DECLARED AS INPUT PORT
}

int usart_init(void)
{


SETBIT(UCSRA,BIT(1));//DOUBLE ASYN MODE ENABLED

SETBIT(UCSRB,BIT(4));//RECEP ENABLED
UCSRC=0X06;//PARITY DISABLED ASYNC 8 BIT
UBRRL=103;//SET BAUD RATE TO 9600 ASYNC
UBRRH=0;
return 0;
}

char recieve_data()
{
	while(!(UCSRA & (1<<RXC)))
	{}
	return UDR;
	CLEARBIT(UCSRA,BIT(7));
}
void delay_ns(unsigned char tim)
{
        for(l=0;l<tim;l++)
        {
                for(n=0;n<10;n++)
                {};
        }
}

void fwd(int speed)
{
        SETBIT(PORTC,BIT(2));      
        CLEARBIT(PORTC,BIT(3));
        SETBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(2));   
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(200-speed);        
}


void back(int speed)
{
       SETBIT(PORTC,BIT(3));      
        CLEARBIT(PORTC,BIT(2));
        SETBIT(PORTC,BIT(5));
        CLEARBIT(PORTC,BIT(4));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(2));   
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(200-speed);    
}



void right(int speed)
{
	    SETBIT(PORTC,BIT(2));      
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(5));
        CLEARBIT(PORTC,BIT(4));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(2));   
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(200-speed);    
}

void left(int speed)
{
	    CLEARBIT(PORTC,BIT(2));      
        CLEARBIT(PORTC,BIT(3));
        SETBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(speed);           
        CLEARBIT(PORTC,BIT(2));   
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
        delay_ns(200-speed);    	
}

void stop(void)
{
        CLEARBIT(PORTC,BIT(2));
        CLEARBIT(PORTC,BIT(3));
        CLEARBIT(PORTC,BIT(4));
        CLEARBIT(PORTC,BIT(5));
}





int main() 
{
	int temp;
	char var;
		port_init();
		PORTD=0XFF;
		//lcd_init();
		PORTC=0XFF;
	
 	while(1)
	{	
		
		//var=recieve_data();
		//temp= PIND;
		//temp=temp & 0b00001111;
		//CLEARBIT(PINC,BIT(6));
		if((bit_is_clear(PIND, 0)) && (bit_is_clear(PIND, 1)) && (bit_is_clear(PIND, 2)) && (bit_is_clear(PIND, 3)))
		{
		stop();
		//lcd_gotoxy1(0);
		//lcd_char('s');
		}
		else if((bit_is_clear(PIND, 0)) && (bit_is_clear(PIND, 1)) && (bit_is_clear(PIND, 2)) && (bit_is_set(PIND, 3)))
		{
		left(150);
		//lcd_gotoxy1(0);
		//lcd_char('f');
		}
		else if((bit_is_set(PIND, 0)) && (bit_is_clear(PIND, 1)) && (bit_is_clear(PIND, 2)) && (bit_is_clear(PIND, 3)))
		{
		back(150);
		//lcd_gotoxy1(0);
		//lcd_char('b');
		}
		else if((bit_is_clear(PIND, 0)) && (bit_is_set(PIND, 1)) && (bit_is_clear(PIND, 2)) && (bit_is_clear(PIND, 3)))
		{
		fwd(150);
		//lcd_gotoxy1(0);
		//lcd_char('r');
		}
		else if((bit_is_clear(PIND, 0)) && (bit_is_clear(PIND, 1)) && (bit_is_set(PIND, 2)) && (bit_is_clear(PIND, 3)))
		{
		right(150);
		//lcd_gotoxy1(0);
		//lcd_char('l');
		}
		
		else
		{
		stop();
		}






		
		
		
		
	}
}

