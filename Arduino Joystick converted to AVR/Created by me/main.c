#include "PS2.h"
#include "LCD_640.h"




int main (void) 
{	
	lcd_init();
	psx_init(&PORTD, 1,&PORTD, 0,&PORTD, 3,&PORTD, 4);
	while(1)
	{
		psx_read_gamepad();
		
		lcd_gotoxy1(0);
		lcd_showvalue(psx_stick(PSS_LX));
		lcd_string("   ");
		lcd_gotoxy1(8);
		lcd_string("LY:");
		lcd_gotoxy1(11);
		lcd_showvalue(psx_stick(PSS_LY));
		lcd_string("   ");
		/*
		
		lcd_gotoxy2(0);
		lcd_string("RX");
		lcd_gotoxy2(3);
		lcd_showvalue(psx_stick(PSS_RX));
		lcd_string("   ");
		
		lcd_gotoxy2(8);
		lcd_string("RY");
		lcd_gotoxy2(11);
		lcd_showvalue(psx_stick(PSS_RY));
		lcd_string("   ");*/
	}
	

}