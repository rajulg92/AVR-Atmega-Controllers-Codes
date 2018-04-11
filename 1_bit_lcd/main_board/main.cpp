/*
	THIS CODE GOES ON MAIN BOARD
*/



#include <avr/io.h>
#include <util/delay.h>

//#include "R_LCD128.h"




int main(void)
{
	DDRB = 0xFF;
	unsigned char ch = 0x01;
	PORTB = 0x00;
	PORTB = 0xFF;
	_delay_ms(500);
	PORTB &= (0<<PORTB0);
	while(1);
	return 0;
}




//#include "SingleBitLCD.h"
/*int main()
{
	lcd_init();
	volatile uint8_t *a = &PORTA, *b = &DDRA, *c = &PINA;
	
	lcd_showvalue(a);
	lcd_gotoxy1(8);
	lcd_showvalue(b);
	lcd_gotoxy2(5);
	lcd_showvalue(c);
	//UartInit();
	//lcd_string("as");
	while(1)
	{
	//	lcd_showvalue( UartReceive());
	}

}*/