#include<avr/io.h>
#include<util/delay.h>
#include"LCD.h"
#include<avr/interrupt.h>
int count=0;

void main()
{
int counter=50;
lcd_init();
while(counter!=0)
{
while(CHECKBIT(PIND,BIT(0)))
{
CLEARBIT(PORTD,BIT(1));
}
counter--;
lcd_gotoxy1(0);
lcd_showvalue(counter);
}
lcd_cmd(LINE2);
lcd_string("Motor Stopped");
SETBIT(PORTD,BIT(1));
}