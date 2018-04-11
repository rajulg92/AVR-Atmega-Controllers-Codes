/*
 * Atmel_Final.c
 *
 * Created: 2/25/2014 12:19:25 PM
 *  Author: YateenKedare
								OUTPUT
 GRIP				PA2			A3
GRIPMECH_UP_DWN		PA1			A6
SUCTION MAN INIT	PA0			A4 cy
 
SEESAW AUTO			PD5			ss PA5		PUMP PE5	
swing auto			PB0			PA0			

PAL UP				PC2
PAL DN				PC3

COM
SWING		PB5
SS			PB6
POLE WA		PB2
LADDER		PB3

auto
bot up seesaw	PB1
bot up swing	PD2
 
 */ 


#define F_CPU 14745600
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "R_LCD640.h"
#define false 0
#define true 1
#define _cb(x,b) x&b
#define _clb(x,b) x&=~b
#define _sb(x,b) x|=b
#define bit(x) (1 << (x))

#include "FUNCTIONS.h"

uint8_t UartReceive(void);
void UartTransmit(uint8_t);
void UartInit(void);
uint8_t lt_st = false, rt_st = false, ppst = false, grip_flag = false;
uint8_t NirmaMech = 0;
void ProcessPINC(uint8_t data)
{
	//lcd_gotoxy1(8);
	//lcd_string("PINC ");
	//lcd_showvalue(data);
	//PAL UP				PC2
	//PAL DN				PC3

	
	if((!(_cb(data,bit(0)))) && (_cb(data,bit(1))))
	{
		rt_st = true;
		rgt_fwd();
	}
	else if((_cb(data, bit(0))) && (!(_cb(data, bit(1)))))
	{
		rt_st = true;
		rgt_rev();
	}
	else if((_cb(data, bit(0))) && (_cb(data,bit(1))))
	{
		if(rt_st == true) 
		{
			rt_st = false;
			rgt_stop();
		}
	}
	if ((!(_cb(data,bit(6)))) && (_cb(data, bit(7))))
	{
		lt_st = true;
		lft_fwd();
	}
	else if((_cb(data,bit(6))) && (!(_cb(data, bit(7)))))
	{
		lt_st = true;
		lft_rev();
	}
	else if((_cb(data, bit(6))) && ((_cb(data, bit(7)))))
	{
		if(lt_st == true)
		{
			lt_st = false;
			lft_stop();
		}
	}
	
	
	if((!(_cb(data, bit(2)))) && (_cb(data, bit(3))))
	{
		ppup();
	}
	else if((_cb(data, bit(2))) && (!(_cb(data, bit(3)))))
	{
		ppdown();
	}
	else if((_cb(data, bit(2))) && (_cb(data, bit(3))))
	{
		ppstop();
	}

}

void ProcessPIND(uint8_t data)
{
	if ( ! (CHECKBIT(data,BIT(5))) )
	{
		seesaw();
	}
	
	if (bit_is_clear(data, 2))				//swing automation
	{
		while(ADCConvert() >= 163)
		{
			ppup();
		}
		ppstop();
	}
}
	
void ProcessPINB(uint8_t data)
{
	
	//SWING			PB5
	//SS			PB6
	//POLE WA		PB2
	//LADDER		PB3
	if(bit_is_clear(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0x08;
	}
	else if(bit_is_set(data, 3) && bit_is_clear(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0x04;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_clear(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0x02;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_clear(data, 6))

	{
		PORTD = 0x01;
	}
	else
	{
		PORTD = 0x00;
	}

	
	if (bit_is_clear(data, 1))				//ss automation
	{
		while(ADCConvert() >= 170)
		{
			ppup();
		}
		ppstop();
	}
	if (bit_is_clear(data, 0))				//pole automation
	{
		while(ADCConvert() >= 194)
		{
			ppup();
		}
		ppstop();
	}
	
}
	
void ProcessPINA(uint8_t data)
{
	//lcd_gotoxy1(0);
	//lcd_string("PINA ");
	//lcd_showvalue(data);
//GRIP				PA2
//GRIPMECH			PA1
//SUCTION MAN INIT	PA0
	if( !(_cb(data,bit(1))) )			//GRIPPER UP DOWN MECH
	{
		SETBIT(PORTA,BIT(6));
	}
	
	else
	{
		CLEARBIT(PORTA,BIT(6));
	}
	
	if ( !(_cb(data,bit(2))) )			// GRIPPER - Pneumatics
	{
		SETBIT(PORTA,BIT(3));
		ReversePWMForPP = 170;
	}
	else
	{
		CLEARBIT(PORTA,BIT(3));
		ReversePWMForPP = 210;
	}
	
	
	if(!(_cb(data, bit(0))))			// suc cups & cyl (to hold the bot at one place)
	{
		SETBIT(PORTE,BIT(5));
	}
	else
	{
		CLEARBIT(PORTE,BIT(5));
	}
}
/*
void ProcessADC( uint8_t data )
{
	lcd_cmd(LINE1);
	lcd_showvalue(data);
	MaxonSpeed = 150 + data*2;
	if (MaxonSpeed > 575)
	{
		MaxonSpeed = 575;
	}
}
*/


int main(void)
{
	lcd_init();
	UartInit();
	port_init();
	pwm_init();
	ADCInit();	
    while(1)
    {
		UartTransmit(50);
		ProcessPINA(UartReceive());
		
		UartTransmit(60);
		ProcessPINB(UartReceive());
		
		UartTransmit(70);
		ProcessPINC(UartReceive());
		
		UartTransmit(80);
		ProcessPIND(UartReceive());
		//lcd_gotoxy1(0);
		//lcd_showvalue(ADCConvert());
		//uarttransmit(90);
		//processadc(uartreceive());
    }
	return 0;
}