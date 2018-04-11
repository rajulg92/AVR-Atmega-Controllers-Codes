
#include <avr/io.h>
#include <util/delay.h>
#include "LCD_640.h"
#include "ADXL345\ADXL345.h"

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