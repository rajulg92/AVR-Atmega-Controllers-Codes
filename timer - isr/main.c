#include<avr/io.h>
#include<util/delay.h>
#include"LCD.h"
#include<avr/interrupt.h>
int count=0;

void timer_init()
{
TCCR1B |= (1<<CS10);
TIMSK |= (1<<TOIE1);
DDRC=0b11111111;
//PORTC=0b0000000;
TCNT1=0;
sei();
}

ISR(TIMER1_OVF_vect)
{
count++;

if(count>=15)
{
count=0;
PORTC ^= (1<<PC0);
}
}

int main()
{

timer_init();
//lcd_init();
while(1);

return 0;
}