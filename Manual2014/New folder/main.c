/*
 * Atmel_Final.c
 *
 * Created: 2/25/2014 12:19:25 PM
 *  Author: YateenKedare
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
#define sen1 0
#define sen2 1
#define sen3 2
#define sen4 3
#define sen5 4
#define pin_sen PINC
#include "FUNCTIONS.h"

uint8_t UartReceive(void);
void UartTransmit(uint8_t);
void UartInit(void);
uint8_t lt_st = false, rt_st = false, discst = false, ppst = false, PidStarted = false, grip_flag = false;

void ProcessPINC(uint8_t data)
{
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
	
	if((!(_cb(data, bit(6)))) && (_cb(data, bit(7))))
	{
		grip_flag = true;
		gripup();
	}
	else if((_cb(data, bit(6))) && (!(_cb(data, bit(7)))))
	{
		grip_flag = true;
		gripdown();
	}
	else if((_cb(data, bit(6))) && (_cb(data, bit(7))))
	{
		if(grip_flag == true)
		{
			grip_flag = false;
			gripstop();
		}
	}
	
}
	
void ProcessPINB(uint8_t data)
{
	//bit 3; bit 5; bit 2; bit 6;
	if(bit_is_clear(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b11111110;
	}
	else if(bit_is_set(data, 3) && bit_is_clear(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b11111101;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_clear(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b11111011;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_clear(data, 6))

	{
		PORTD = 0b11110111;
	}
	else
	{
		PORTD = 0b11111111;
	}
}
	
void ProcessPINA(uint8_t data)
{

	if( !(_cb(data,bit(1))) )
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
		ReversePWMForPP = 40;
	}
	else
	{
		CLEARBIT(PORTA,BIT(3));
		ReversePWMForPP = 110;
	}
	
	if(!(_cb(data, bit(0))))			// suc cups & cyl (to hold the bot at one place)
	{
		SETBIT(PORTE,BIT(4));
		_delay_ms(100);
		CLEARBIT(PORTA,BIT(5));
	}
	else
	{
		CLEARBIT(PORTE,BIT(4));
		_delay_ms(100);
		SETBIT(PORTA,BIT(5));
		
	}
	
	
	if(!(_cb(data, bit(4))))
	{
		PidStarted = true;
		pid();
	}
	else 
	{
		if(PidStarted == true)
		{
			PidStarted = false;
			pid_stop();
		}
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
		
		//uarttransmit(90);
		//processadc(uartreceive());
    }
	return 0;
}