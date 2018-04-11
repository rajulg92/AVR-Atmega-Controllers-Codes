//*****************************************************************************
//
// File Name	: 'lcd_lib.c'
// Title		: 8 and 4 bit LCd interface
// Author		: Scienceprog.com - Copyright (C) 2007
// Created		: 2007-03-29
// Revised		: 2007-08-08
// Version		: 1.0
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
#include "lcd_lib.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <compat/deprecated.h>

const uint8_t LcdCustomChar[] =//define 8 custom LCD chars
{
	0x1F, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, //Char0
	0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, //Char1
	0x00, 0x00, 0x04, 0x04, 0x14, 0x14, 0x14, 0x14, //Char2
	0x01, 0x01, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, //Char3
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char4
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char5
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char6
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char7
};

void LCDtoggleEnable(void)
{
	sbi(LCD_CONT_PORT,LCD_EN);
	_delay_ms(3);
	cbi(LCD_CONT_PORT,LCD_EN);
}

void LCDsendChar(uint8_t data)
{
	sbi(LCD_CONT_PORT,LCD_RS);
	
	LCD_DATA_PORT = data;
	
	LCDtoggleEnable();
	
	_delay_ms(10);
}

void LCDsendCommand(uint8_t command)
{
	cbi(LCD_CONT_PORT,LCD_RS);
	
	LCD_DATA_PORT = command;
	
	LCDtoggleEnable();
	
	_delay_ms(10);
}

void LCDinit(void)//Initializes LCD
{
	LDDR = 0xFF;
	LCDR |= 1<<LCD_EN|1<<LCD_RS;
	
	LCDsendCommand(0x38);
	_delay_ms(10);
	
	LCDsendCommand(0x01);
	_delay_ms(10);
	
	LCDsendCommand(0x06);
	_delay_ms(10);
	
	LCDsendCommand(0x0C);
	_delay_ms(10);
	
	LCDsendCommand(0x80);
	_delay_ms(10);

}	
		
void LCDclr(void)				//Clears LCD
{
	LCDsendCommand(0x01);
}

void LCDhome(void)			//LCD cursor home
{
	LCDsendCommand(0x02);
}

void LCDstring(uint8_t *string, uint8_t nbytes)	//Outputs string to LCD
{
	while (*string)
	LCDsendChar(*string++);
}

void LCDGotoXY(uint8_t x, uint8_t y)	//Cursor to X Y position
{
	register uint8_t DDRAMAddr;
	// remap lines into proper order
	switch(y)
	{
	case 0: DDRAMAddr = LCD_LINE0_DDRAMADDR+x; break;
	case 1: DDRAMAddr = LCD_LINE1_DDRAMADDR+x; break;
	case 2: DDRAMAddr = LCD_LINE2_DDRAMADDR+x; break;
	case 3: DDRAMAddr = LCD_LINE3_DDRAMADDR+x; break;
	default: DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
	}
	// set data address
	LCDsendCommand(1<<LCD_DDRAM | DDRAMAddr);
	
}

void LCDcustom(void)
{
	char i;
	LCDsendCommand(0x40);
	
	for(i=0; i<64; i++)
	{
		LCDsendChar(LcdCustomChar[i]);
	}
	
	LCDsendCommand(0x80);
}	

