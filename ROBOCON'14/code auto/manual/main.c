/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
 
/* 
	BUTTON ACTIONS
		Left arrow keys ---- all motions
		
		tri -- pully UP
		X	-- pully down
		O	-- PID
		squ	-- gripper pneu + gripper suction
		
		L1, R1	--	gripper motor
		L2, R2	--	disc
		
		select	--	suction on + (seasaw pnew)x3 + suction off
		
		combinations available for now(will add more if needed)
		
		1. PID + disc				done
		2. Disc + Pully				done
		
*/
 /*
	Some stuff for Ayush and "Stud" Pathak
	Code has been divided into two parts for now because of the shitty joystick.
	the commented part at the bottom allows controller to use left side direction controls of the joystick
	since that is not working properly the motions controls have been shifted
	
	The PID part is actually curve avoiding rather than curve tracing but it works quite smoothly
	only with constant ‘p’. I will try to increase the speed of this so called “curve tracing” 
	tracing algorithm (For some reasons mention it as curve tracing only) 
	(Ayush if u are trying to do that then increase the value of ‘p’ proportionately to increase in speed and 
	try to add the constant'd' to increase smoothness)
*/
 
 

#include <avr/io.h>
#include <util/delay.h>
#include "joystick.h"
//#include "LCD640.h"
#define sen1 6		//left extreme
#define sen2 5
#define sen3 2
#define sen4 3
#define sen5 4		//right extreme
#define PIN_sen PINC
#define DeBouncingConstant 14*(10^4)
#define true 1
#define false 0

uint8_t data = 0, err = 0, acc_err = 0, inst_err = 0, prev_err = 0;
float p = 20, i = 0, d = 0;
float corr = 0;
int dir = 'f', motion;
int debonc_f = 0, debonce_r = 0;


void port_init()
{
	DDRE |= (1 << PE3) | (1 << PE4);
	DDRH |= (1 << PH3) | (1 << PH4);
	DDRJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3);
	DDRG |= (1 << PG4) | (1<<PG3);
	DDRL |= (1 << PL0) | (1<<PL2);
	DDRA |= (1<<PA3) | (1<<PA5) | (1<<PA1) | (1 << PA0) | (1<<PA2) | (1<<PA4);
	DDRB |= (1<<PB5);
	PORTA &= ~(1<<PA3) & ~(1<<PA5) & ~(1<<PA1) & ~(1<<PA0)& ~(1<<PA2)& ~(1<<PA4);
	DDRC = 0x00;
	PORTC = 0xFF;
}

void pwm_init()
{
	TCCR1A = 0xA8;
	TCCR1B = 0x11;
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR3 = 737;
	ICR4 = 737;
	ICR1 = 737;
}

void leftFwd(int x) {
	OCR3A = x;
	PORTJ |= (1 << PJ1);
	PORTJ &= ~(1 << PJ0);
}

void leftRev(int x) {
	OCR3A = x;
	PORTJ |= (1 << PJ0);
	PORTJ &= ~(1 << PJ1);
}

void rightFwd(int x) {
	OCR3B = x;
	PORTJ |= (1 << PJ2);
	PORTJ &= ~(1 << PJ3);
}

void rightRev(int x) {
	OCR3B = x;
	PORTJ |= (1 << PJ3);
	PORTJ &= ~(1 << PJ2);
}

void Stop() {
	OCR3A = 100;
	OCR3B = 100;
}

void pid()
{
	//get the sensor input

	if(bit_is_set(PIN_sen, sen1))
		err = 5;
	else if(bit_is_set(PIN_sen, sen2))
		err = 4;
	else if(bit_is_set(PIN_sen, sen3))
		err = 3;
	else if(bit_is_set(PIN_sen, sen4))
		err = 2;
	else if(bit_is_set(PIN_sen, sen5))
		err = 1;
	else
		err = 0;
	inst_err = err - prev_err;
	corr = p*err + i*acc_err + d*inst_err;
	acc_err += err;
	prev_err = err;

	OCR3A = 250 + (int)(ceil(corr));
	OCR3B = 350 - (int)(ceil(corr));
}
/*PC2 = err 0
  PC3 = err 1
  PC4 = err 2
  PC5 = err -1
  PC6 = err -2
*/

int main(void)
{
	//lcd_init();
	int pullyu = false, pullyd = false, discl = false, discr = false, pidf = false, pidf1 = false;
	int pullyu1 = false, pullyd1 = false, discl1 = false, discr1 = false, pullyu2 = false, pullyd2 = false, grip_up = false, grip_dwn = false;
    port_init();
	pwm_init();
	ps2_init();

	joystick_config();
	joystick_config();
	joystick_config();
	
	OCR4A = 0;
	OCR4B = 0;
	OCR3C = 0;
	
	Stop();

    for(;;){
		
		joystick_poll();
	    /*lcd_gotoxy1(0);
		lcd_string("data0 : ");
		lcd_showvalue(data0);
		lcd_string("   ");

		lcd_gotoxy2(0);
		lcd_string("data1 : ");
		lcd_showvalue(data1);
		lcd_string("   ");*/

		
		if(data0 == 136 || data0 == 8) {			//motion fwd
			leftFwd(400);
			rightFwd(400);
			dir = 'f';
			motion = true;
			
		}

		else if(data0 == 56) {		//motion rev
			leftRev(400);
			rightRev(400);
			dir = 'r';
			motion = true;
		}

		else if(data0 == 24) {		//motion right
			if(dir == 'f')
			{
				rightFwd(450);
				leftFwd(170);
			}
			else if(dir == 'r')
 			{
				rightRev(450);
				leftRev(170);
			}
			motion = true;
		}

		else if(data0 == 120) {		//motion left
			if(dir == 'f')
			{
				leftFwd(450);
				rightFwd(160);
			}
			else if(dir == 'r')
			{
				leftRev(450);
				rightRev(160);
			}
			motion = true;
		}
		else 											//motion stop
		{
			if(motion == true)
			{
				Stop();
				motion = false;
			}
		}
		
		if(data1 == 31 && data0 == 248) {								//curve tracing on circle
			pidf = true;
			dir = 'f';
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
		}
		
		else if(data1 == 255 && data0 == 248) {
			if(pidf == true) {
				pidf = false;
				Stop();
			}
			
		}	
		
		
		if ((data1 == 241 || data1 == 225)) {		//R2 disc right
			discl = true;
			PORTG &= ~(1 << PG4);
			OCR4A = 700;
		}

		else if((data1 == 240 || data1 == 248))  {			//L2 disc left
			discr = true;
			PORTG |= (1 << PG4);
			OCR4A = 700;
		}
		
		else if(data1 == 240 && (data0 == 120, data0 == 136, data0 == 24, data0 == 56)) {			//L2 disc left
			discr = true;
			PORTG |= (1 << PG4);
			OCR4A = 700;
		}

		else if(data1 == 255) {
			if(discl == true || discr == true) {
				discl = false;
				discr = false;
				OCR4A = 0;
			}
		}
		
		if (data1 == 199) {		//R1 gripper up
			grip_up = true;
			OCR1A = 700;
			//PORTB |= (1 <<PB5);
			PORTL &= ~(1 << PL2);
		}

		else if(data1 == 227) {			//L1 gripper down
			grip_dwn = true;
			OCR1A = 700;
			//PORTB |= (1<<PB5);
			PORTL |= (1 << PL2);
		}

		else {
			//if(grip_up == true || grip_dwn == true) {
				grip_up = false;
				grip_dwn = false;
				OCR1A = 0;
			//}
			
		}
		  
		  
		if(data1 == 143) {							//tri pullyup
			pullyu = true;
			OCR4B = 700;
			PORTL |= (1 << PL0);
		}

		else if(data1 == 63) {						// X  pulludown
			pullyd = true;
			OCR4B = 700;
			PORTL &= ~(1 << PL0);
		}
		else
		{
			if(pullyu == true || pullyd == true)
			{
				PORTL |= (1<<PL0);
				OCR4B = 40;
				pullyu = false;
				pullyd = false;
			}
		}
		 
 
		if(data0 == 240 && data1 == 255 )			//seasaw + vaccume pneu on SELECT
		{											//PRESS ONLY ONCE FOR 3 MOTIONS OF SEASAW
			PORTA |= (1<<PA3);
			PORTA |= (1<<PA5);
			for(int i = 0; i < 3; i++)
			{
				PORTA |= (1<<PA1);
				_delay_ms(1000);
				PORTA &= ~(1<<PA1);
				_delay_ms(1000);
			}
			PORTA &= ~(1<<PA3);
			PORTA &= ~(1<<PA5);
		}
		else
		{
			PORTA &= ~(1<<PA1);
			PORTA &= ~(1<<PA3);
			PORTA &= ~(1<<PA5);
		}
		
		if(data1 == 127) {
			PORTA ^= (1 << PA5)
			PORTA ^= (1 << PA0);
			PORTA ^= (1 << PA2);
			PORTA ^= (1 << PA4);
		}


		if((data1 == 1 || data1 == 17) && data0 == 248)					//PID + disk combination
		{
			pidf1 = true;
			dir = 'f';
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
			discr1 = true;
			OCR4A = 700;
			PORTG &= ~(1 << PG4);
		}
		else if((data1 == 16|| data1 == 24) && data0 == 248)
		{
			pidf1 = true;
			dir = 'f';
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
			discl1 = true;
			OCR4A = 700;
			PORTG |= (1 << PG4);
		}
		else
		{
			if(pidf1 == true) {
				pidf1 = false;
				Stop();
			}
			if(discl1 == true || discr1 == true) {
				discl1 = false;
				discr1 = false;
				OCR4A = 0;
			}
		}
		
		
/*		if()					//PID + PULLY combination
		{
			pidf1 = true;
			dir = 'f';
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
			pullyu1 = true;
			OCR4B = 700;
			PORTL |= (1 << PL0);
		}
		else if()
		{
			pidf1 = true;
			dir = 'f';
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
			pullyd1 = true;
			OCR4B = 700;
			PORTL &= ~(1 << PL0);
		}
		else
		{
			if(pidf1 == true) {
				pidf1 = false;
				Stop();
			}
			if(pullyu1 = true || pullyd1 = true) {
				pullyu1 = false;
				pullyd1 = false;
				PORTL |= (1<<PL0);
				OCR4B = 40;
			}
		}
*/		
		if(data0 == 248 && data1 == 129)		//discr + pully
		{
			discr1 = true;
			OCR4A = 600;
			PORTG &= ~(1 << PG4);
			pullyu2 = true;			//pully up
			OCR4B = 700;
			PORTL |= (1 << PL0);
		}
		else if(data0 == 248 && (data1 == 33 || data1 == 49))
		{
			discr1 = true;
			OCR4A = 600;
			PORTG &= ~(1 << PG4);
			pullyd2 = true;			//pully down
			OCR4B = 700;
			PORTL &= ~(1 << PL0);
		}
		
		else if(data0 == 248 && data1 == 128)		//discl + pully
		{
			pullyu2= true;			//pully up
			OCR4B = 700;
			PORTL |= (1 << PL0);
		}
		else if(data0 == 248 && data1 == 48)
		{
			pullyd2 = true;			//pully down
			OCR4B = 700;
			PORTL &= ~(1 << PL0);
		}
		else
		{
			if(pullyu2 == true || pullyd2 == true)
			{
				PORTL |= (1<<PL0);
				OCR4B = 40;
				pullyu2 = false;
				pullyd2 = false;
			}
			if(discr1 == true || discl1 == true) {
				discl1 = false;
				discr1 = false;
				OCR4A = 0;
			}
		}
		
		
		
    }
    return 0;   /* never reached */
}
