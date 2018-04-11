/*
 * LCDDemo.c
 *
 * Created: 15/06/2014 09:50:02 AM
 *  Author: Avinash
 */ 


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lib/lcd/lcd_hd44780_avr.h"

int main(void)
{
	LCDInit(LS_NONE);
	
	LCDWriteFString(PSTR("LCD Test !"));
	LCDWriteFStringXY(0,1,PSTR("By Avinash"));
	
	//infinite loop
	while(1);
}