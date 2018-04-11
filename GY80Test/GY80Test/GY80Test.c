/*
 * GY80Test.c
 *
 * Created: 7/26/2014 10:52:00 AM
 *  Author: YateenKedare
 */ 

F_CPU = 14745600
#include <util/delay.h>
#include "ADXL345\ADXL345.h"
#include "LCD_640.h"
int main(void)
{
	_delay_ms(500);
	lcd_init();
	begin();
	lcd_gotoxy1(0);
	lcd_string("X:");
	while(1)
	{
		readAccel();
		lcd_gotoxy1(3);
		lcd_showvalue(ADXL345_Xaxis);
		lcd_gotoxy2(3);
		lcd_showvalue(ADXL345_Yaxis);
	}
}