#include <avr/io.h>
#include <avr/delay.h>
#include "LCD.h"

#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;

int x1=0,temp=0;

void adc_init()
{
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= (1<<MUX0);

}

int conv_adc()
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



int main()
{
int temp=0,temp2=0;

DDRC=0X00;
DDRB=0XFF;
DDRD=0XFF;
lcd_init();
adc_init();


while(1)
{

temp=conv_adc();
lcd_gotoxy2(0);
lcd_showvalue(temp);

ADMUX++;
temp2=conv_adc();
lcd_gotoxy2(6);
lcd_showvalue(temp2);


}


}