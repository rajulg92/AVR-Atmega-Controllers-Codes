/*
 * OmniAlpha.c
 *
 * Created: 4/19/2014 10:15:09 AM
 *  Author: YateenKedare
 */ 
#define F_CPU 14745600

#include "PS2.h"
#include "LCD_640.h"
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
	
	lcd_init();
	psx_init(&PORTD, 1,&PORTD, 0,&PORTD, 3,&PORTD, 4);
	pwm_init();
	while(1)
	{
		psx_read_gamepad();
		OmniDrive( Get_Distance ( psx_stick(PSS_LX), psx_stick(PSS_LY) ),
					Get_Angle( psx_stick(PSS_LX), psx_stick(PSS_LY) ),
					  (psx_stick(PSS_RX) - 123)*7);
			
	}
	
	
	/*while (1)
	{
		psx_read_gamepad();
		
		if (psx_button(PSB_PAD_UP) == 1)
		{
			OmniDrive(737, 0, 0);
		}
		
		if ((psx_button(PSB_PAD_LEFT)) == 1)
		{
			OmniDrive(737, 90, 0);
		}
		if ((psx_button(PSB_PAD_RIGHT)) == 1)
		{
			OmniDrive(737, -90, 0);
		}
		if ((psx_button(PSB_PAD_DOWN)) == 1)
		{
			OmniDrive(737, 180, 0);
		}
		if ((psx_button(PSB_TRIANGLE)) == 1)
		{
			OmniDrive(737, -45, 0);
		}
		if ((psx_button(PSB_CIRCLE)) == 1)
		{
			OmniDrive(737, -135, 0);
		}
		
		if ((psx_button(PSB_CROSS)) == 1)
		{
			OmniDrive(737, 135, 0);
		}
		if ((psx_button(PSB_SQUARE)) == 1)
		{
			OmniDrive(737, 45, 0);
		}
		
		if ((psx_button(PSB_L1)) == 1)
		{
			OmniDrive(0,0,737);
		}
		if ((psx_button(PSB_R1)) == 1)
		{
			OmniDrive(0,0,-737);
		}
		
		if ((psx_button(PSB_R1)) == 0 && (psx_button(PSB_L1)) == 0 && (psx_button(PSB_SQUARE)) == 0 && (psx_button(PSB_CROSS)) ==0  && (psx_button(PSB_CIRCLE)) == 0 && (psx_button(PSB_TRIANGLE)) == 0 && (psx_button(PSB_PAD_LEFT)) == 0 && (psx_button(PSB_PAD_DOWN)) == 0 && (psx_button(PSB_PAD_RIGHT)) == 0 && (psx_button(PSB_PAD_UP)) == 0)
		{
			OmniStop();
		}
	}*/
}


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