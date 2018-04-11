/*
 * Atmel_Final.c
 *
 * Created: 2/25/2014 12:19:25 PM
 *  Author: Coder Rajat
 */ 

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
#define F_CPU 14745600

uint8_t UartReceive(void);
void UartTransmit(uint8_t);
void UartInit(void);
float p = 6, i = 0.0, d = 0.0, corr = 0, prev_err = 0, acc_err = 0, inst_err = 0, err = 0;
uint8_t swingauto = false, flag1 = false;

void pid(void)
{
	//get the sensor input
	if((_cb(PINC, bit(3))))
	{
		err = 4;
	}
	else if(_cb(PINC, bit(3)))
		err = 3;
	else if(_cb(PINC, bit(2)))
		err = 2;
	else if((_cb(PINC, bit(1))))
		err = 1;
	else if((_cb(PINC, bit(0))))
		err = 0;
	else 
		err =-4;
	
	inst_err = err - prev_err;
	corr = p*err + i*acc_err + d*inst_err;
	acc_err += err;
	prev_err = err;

	PidStarted = true;

	PORTJ |= (1 << PJ0);
	PORTJ &= ~(1 << PJ1);
	PORTJ |= (1 << PJ2);
	PORTJ &= ~(1 << PJ3);
		
	OCR5B = 270 + (int)(ceil(corr));
	OCR5A = 345 - (int)(ceil(corr));	
}

void pid_stop(void)
{
	lft_stop();
	rgt_stop();
	inst_err = 0;
	corr = 0;
	prev_err = 0;
	acc_err = 0;
}

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
				ppst = false;
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
	if(!(_cb(data, bit(1))))
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
	
	//bit 3; bit 5; bit 2; bit 6;
	if(bit_is_clear(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b00000001;
	}
	else if(bit_is_set(data, 3) && bit_is_clear(data, 5) && bit_is_set(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b00000010;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_clear(data, 2) && bit_is_set(data, 6))
	{
		PORTD = 0b00000100;
	}
	else if(bit_is_set(data, 3) && bit_is_set(data, 5) && bit_is_set(data, 2) && bit_is_clear(data, 6))

	{
		PORTD = 0b00001000;
	}
	else
	{
		PORTD = 0x00;;
	}
}
	
void ProcessPINA(uint8_t data)
{
	if(!(_cb(data, bit(0))))			// suc cups & cyl (to hold the bot at one place)
	{
		SETBIT(PORTE,BIT(3));
		CLEARBIT(PORTA,BIT(5));
	}
	else
	{
		CLEARBIT(PORTE,BIT(3));
		SETBIT(PORTA,BIT(5));
	}
	
	if (!(_cb(data,bit(0))))
	{
		SETBIT(PORTE,BIT(3));	// Pump
		SETBIT(PORTA,BIT(2));	//Suction Cup
	}
	else
	{
		CLEARBIT(PORTE,BIT(3));	// Pump
		CLEARBIT(PORTA,BIT(2)); // Suction Cup
	}

	if(!(_cb(data, bit(4))))
	{
		SETBIT(PORTA, BIT(7));
		SETBIT(PORTA, BIT(3));
		
	}
	else
	{
		CLEARBIT(PORTA, BIT(7));
		CLEARBIT(PORTA, BIT(3));
		return;
	}
	
	if( !(_cb(data,bit(1))))
	{ 
		SETBIT(PORTA,BIT(7));		// Up Down Gripper Motion
	}
	else
	{
		CLEARBIT(PORTA,BIT(7));
	}
		
	if ( !(_cb(data,bit(2))) )			// GRIPPER - Pneumatics
	{
		SETBIT(PORTA,BIT(3));
		ReversePWMForPP = 40;			// So that when auto is gripped ... Reverse is Increased
	}
	else
	{
		CLEARBIT(PORTA,BIT(3));
		ReversePWMForPP = 40;
	}
	

}

void ProcessADC( uint8_t data )
{
	return;
	MaxonSpeed = 140 + data*2;
	if (MaxonSpeed > 575)
	{
		MaxonSpeed = 575;
	}
}


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
		
		//UartTransmit(90);
		//ProcessADC(UartReceive());
    }
	return 0;
}