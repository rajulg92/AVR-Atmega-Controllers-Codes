#include<avr/io.h>
#include<util/delay.h>
#include"LCD.h"
int count=0;

void timer_init()
{
TCCR1B |= (1<<CS10);
DDRC=0b11111111;
//PORTC=0b0000000;
TCNT1=0;
}

void delay_s(float num) /// Timer exceeds by 400ms.
{
while(1)
{
if(TCNT1>50000)
{
	count++;
	TCNT1=0;
	if(count>250*num)
	{
		count=0;
		break;
	}
}
}
}

int main()
{

timer_init();
//lcd_init();
while(1)
{
PORTC=0XFF;
PORTB=0XFF;
PORTA=0XFF;
PORTD=0XFF;
PORTE=0XFF;
PORTF=0XFF;
PORTG=0XFF;
PORTH=0XFF;
PORTJ=0XFF;
PORTK=0XFF;
PORTL=0XFF;
delay_s(1);
PORTC=0X00;
PORTB=0X00;
PORTA=0X00;
PORTD=0X00;
PORTE=0X00;
PORTF=0X00;
PORTG=0X00;
PORTH=0X00;
PORTJ=0X00;
PORTK=0X00;
PORTL=0X00;
delay_s(1);
}

//lcd_char('A');
while(1);

return 0;
}