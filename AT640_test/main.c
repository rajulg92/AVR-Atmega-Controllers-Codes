#include<avr/io.h>
#include "LCD640.h"
int i = 0;
int main(void)
{
	
	lcd_init();
	while(1)
	{
		for(i = 0; i< 8; i++)
		{
			lcd_showvalue(0 | _BV(i));
			
		}
	}
}