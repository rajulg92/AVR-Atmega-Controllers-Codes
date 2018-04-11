/*
 * _3WheelOmni.c
 *
 * Created: 4/24/2014 1:34:52 PM
 *  Author: YateenKedare
 */ 
int abc = 8;
int cde = 100;
int def = 1000;
#define F_CPU 14745600
#include "PS2.h"
#include "LCD_640.h"
uint8_t Direction_Y = 0;
#include "USART.h"
#include "OmniFunctions.h"


void pwm_init(void)
{
	SETBIT(DDRE, BIT(3));
	SETBIT(DDRE, BIT(4));
	SETBIT(DDRH, BIT(3));
	SETBIT(DDRH, BIT(4));
	SETBIT(DDRH, BIT(6));
	SETBIT(DDRH, BIT(7));
	SETBIT(DDRG, BIT(4));
	SETBIT(DDRL, BIT(0));

	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR4 = 737;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	ICR3 = 737;
}



int main(void)
{
	_delay_ms(10);
	lcd_init();
	sei();
	psx_init(&PORTA, 1,&PORTA, 0,&PORTA, 3,&PORTA, 4);
	pwm_init();
	Usart_init();
	Timer_init();
	Interruot_Init();
	_delay_ms(10);
	
	
	while(1)
	{
		psx_read_gamepad();
		//OmniDrive3Wheel( Get_Distance ( psx_stick(PSS_LX), psx_stick(PSS_LY) )*6.5,
					//Get_Angle( psx_stick(PSS_LX), psx_stick(PSS_LY) ),
					  //(psx_stick(PSS_RX) - 123)*6);
		lcd_gotoxy1(0);
		lcd_string("LX:");
		//lcd_showvalue(psx_stick(PSS_LX));
		lcd_showvalue(psx_button(PSB_CROSS));
		lcd_char(' ');
		lcd_string("LY:");
		lcd_showvalue(psx_stick(PSS_LY));
		lcd_char(' ');

		lcd_gotoxy2(0);
		lcd_string("RX:");
		lcd_showvalue(psx_stick(PSS_RX));
		lcd_char(' ');
		lcd_string("RY:");
		lcd_showvalue(psx_stick(PSS_RY));
		lcd_char(' ');
	}
}

//
//ISR(INT0_vect)
//{
	//
	//if (((bit_is_set(PIND, 0)) && (bit_is_set(PIND, 1)) ) || ( (bit_is_clear(PIND, 0)) && (bit_is_clear(PIND, 1)) ) )
	//{
		//Direction_Y = 1;
	//}
	//else
	//{
		//Direction_Y = 2;
	//}
	//return;
//}
//
//
//ISR(INT1_vect)
//{
	//if (((bit_is_set(PIND, 0)) && (bit_is_set(PIND, 1)) ) || ( (bit_is_clear(PIND, 0)) && (bit_is_clear(PIND, 1)) ) )
	//{
		//Direction_Y = 2;
	//}
	//else
	//{
		//Direction_Y = 1;
	//}
	//return;
//}
//

/*

	lcd_gotoxy1(0);
	lcd_showvalue(OCR3A);
	lcd_char(' ');
	if (bit_is_set(PORTH, 6))
	{
		lcd_char('1');
	}
	else
	{
		lcd_char('0');
	}
	lcd_char(' ');
	lcd_showvalue(OCR3B);
	lcd_char(' ');
	if (bit_is_set(PORTH, 7))
	{
		lcd_char('1');
	}
	else
	{
		lcd_char('0');
	}
	lcd_gotoxy2(0);
	lcd_showvalue(OCR4A);
	lcd_char(' ');
	if (bit_is_set(PORTG, 4))
	{
		lcd_char('1');
	}
	else
	{
		lcd_char('0');
	}
	lcd_char(' ');
	lcd_showvalue(OCR4B);
	lcd_char(' ');
	if (bit_is_set(PORTL, 0))
	{
		lcd_char('1');
	}
	else
	{
		lcd_char('0');
	}
*/
/*

lcd_gotoxy1(0);
lcd_string("LX:");
lcd_showvalue(psx_stick(PSS_LX));
lcd_char(' ');
lcd_string("LY:");
lcd_showvalue(psx_stick(PSS_LY));
lcd_char(' ');

lcd_gotoxy2(0);
lcd_string("RX:");
lcd_showvalue(psx_stick(PSS_RX));
lcd_char(' ');
lcd_string("RY:");
lcd_showvalue(psx_stick(PSS_RY));
lcd_char(' ');*/