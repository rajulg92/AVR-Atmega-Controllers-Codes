/*
 * PORT.H
 *
 *  Created on: Jan 4, 2011
 *      Author: som
 */

#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;


/************************************************/
void ports_init(void)
{
DDRC=0XFF;
DDRB=0XFF;
DDRD=0XFF;
DDRA=0XFF;
}
/********************************************/


