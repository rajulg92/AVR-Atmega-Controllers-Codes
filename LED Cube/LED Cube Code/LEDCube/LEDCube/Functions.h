#include <avr/io.h>
#define SETBIT(x,b)  x|=(1<<(b));
#define CLEARBIT(x,b)  x&=~(1<<(b));
#define TOGGLEBIT(x,b)  x^=(1<<(b));


void Cube_init(void)
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	PORTD = 0xFF;
	PORTC = 0x02;
}
	//8 9 7 1 3 5 6 4 2
void LED_num_off(int num)
{
	switch (num)
	{
		case 1:	SETBIT(PORTD,7);
		break;
		case 2:	SETBIT(PORTC,1);
		break;
		case 3:	SETBIT(PORTD,6);
		break;
		case 4:	SETBIT(PORTD,0);
		break;
		case 5:	SETBIT(PORTD,2);
		break;
		case 6:	SETBIT(PORTD,4);
		break;
		case 7:	SETBIT(PORTD,5);
		break;
		case 8:	SETBIT(PORTD,3);
		break;
		case 9:	SETBIT(PORTD,1);
		break;
	}
}

void LED_num_on(int num)
{
	switch (num)
	{
		case 1:	CLEARBIT(PORTD,7);
		break;
		case 2:	CLEARBIT(PORTC,1);
		break;
		case 3:	CLEARBIT(PORTD,6);
		break;
		case 4:	CLEARBIT(PORTD,0);
		break;
		case 5:	CLEARBIT(PORTD,2);
		break;
		case 6:	CLEARBIT(PORTD,4);
		break;
		case 7:	CLEARBIT(PORTD,5);
		break;
		case 8:	CLEARBIT(PORTD,3);
		break;
		case 9:	CLEARBIT(PORTD,1);
		break;
	}
}

void LED_off(int level, int num)
{
	switch (level)
	{
		case 0:
			CLEARBIT(PORTC,0);
			LED_num_off(num);
		break;
		case 1:
			CLEARBIT(PORTC,2);
			LED_num_off(num);
		break;
		case 2:
			CLEARBIT(PORTC,4);
			LED_num_off(num);
		break;
	}
}

void LED_On(int level, int num)
{
	switch (level)
	{
		case 0:
			SETBIT(PORTC,0);
			CLEARBIT(PORTC,2);
			CLEARBIT(PORTC,4);
			LED_num_on(num);
		break;
		case 1:
			CLEARBIT(PORTC,0);
			SETBIT(PORTC,2);
			CLEARBIT(PORTC,4);
			LED_num_on(num);
		break;
		case 2:
			CLEARBIT(PORTC,0);
			CLEARBIT(PORTC,2);
			SETBIT(PORTC,4);
			LED_num_on(num);
		break;
	}
	_delay_us(800);
	LED_off(level,num);
}

void LED_hor_plane_on(int level)
{
	for (int i = 1; i <10; i++)
	{
		LED_On(level,i);
	}	
}
