/*#include<avr/io.h>
#include<avr/interrupt.h>
#include "LCD.h"
#include "PORT.h"
unsigned int temp=1;

void main()
{

DDRF=0x00;
//CLEARBIT(PORTF,BIT(2));
lcd_init();
//sei();
//SREG=0x80;
ADCSRA |= 1<<ADPS2;
ADMUX |= (1<<REFS0) | (1<<ADLAR);
ADCSRA |=1<<ADIE;
ADCSRA |=1<<ADEN;
sei();
ADCSRA |=1<<ADSC;
for(;;)
{
}

}

ISR(ADC_vect)
{
temp=ADCH;
lcd_showvalue(temp);
//ADMUX=BIT(5)+BIT(6);
}

*/

//ADMUX 	01100000
//ADCSRA 	10000111

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdlib.h>
#include "LCD.h"
#include "PORT.h"
int temp;
char adcresult[4];

void init_adc()
{
ADMUX=0b01000000;

ADCSRA=0b10000111;
}

int read_adc()
{
uint8_t thelow;
uint16_t ten; 
ADCSRA |= (1<<ADSC);

while(!(ADCSRA & (1<<ADIF)))
{
;
}


thelow = ADCL;
ten = ADCH <<2 | thelow >> 6; 



ADCSRA |= (1<<ADIF);

return(thelow);

}

void main()
{
uint16_t result=0;
int x=0,y=0;
lcd_init();
init_adc();
//lcd_gotoxy1(0);
//lcd_char('Y');
//lcd_gotoxy2(0);
//lcd_char('X');


while(1)
{
result=read_adc();
y=result;
_delay_ms(20);
ADMUX++;
result=read_adc();
x=result;
_delay_ms(20);
ADMUX--;
lcd_gotoxy1(0);
if(0b10100000>=x && x>=0b10011110)
{
	if(0b10100000>=y && y>=0b10011110)
	{
		lcd_string("C ");//Centre
	}
	else if(y>=0b10100010)
	{
		lcd_string("S ");//North
	}
	else if(y<=0b01001000)
	{
		lcd_string("N ");//south
	}
}
else if(0b10100000>=y || y>=0b10011110)
{
	if(x>=0b10100001)
	{
		lcd_string("E ");//east
	}
	else if(x<=0b01000100)
	{
		lcd_string("W ");//west
	}
}
/*else if(0b10100100>=x>=0b10100001 	&&	0b10100001<=y<=0b10100100)
{
	lcd_string("NE");
}
else if(0b00100011<=x<=0b01011111 	&&	0b10100001<=y<=0b10100100)
{
	lcd_string("NW");
}

else if(0b00100011<=x<=0b01011111	&&	0b01011111>=y>=0b00100011)
{
	lcd_string("SW");
}
else if(0b10100100>=x>=0b10100001 	&&	0b01011111>=y>=0b00100011)
{
	lcd_string("SE");
}
*/
}
}
