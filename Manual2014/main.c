/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */
/*
MAXON oc3a(j0, j1)	oc3b(j2, j3)
disk 4a	g4
pully 4b l0
*/
#include <avr/io.h>
#include <util/delay.h>
#include "joystick.h"
//#include "LCD640.h"

#define true 1
#define false 0

#define sen1 6		//left extreme
#define sen2 5
#define sen3 2
#define sen4 3
#define sen5 4		//right extreme

int err_sen = 0, bml, bmr;
int kp = 40, ki = 1, kd = 1;
int acc_err = 0, inst_err = 0, prev_err = 0, bal = 0;
void port_init()
{
	DDRE |= (1 << PE3) | (1 << PE4);
	DDRH |= (1 << PH3) | (1 << PH4);
	DDRJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3);
	DDRG |= (1 << PG4);
	DDRL |= (1 << PL0);
}

void pwm_init()
{
	TCCR3A = 0xA8;
	TCCR3B = 0x11;
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
	ICR3 = 737;
	ICR4 = 737;
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
	OCR3A = 145;
	OCR3B = 145;
}

void pid()
{
	//get the sensor input
	err_sen = error();
	inst_err = err_sen - prev_err;
	bal = kp*err_sen + ki*acc_err + kd*inst_err;
	prev_err = err_sen;
	acc_err += err_sen;
	
	bml = 400 + ceil(bal);
	bmr = 400 - ceil(bal);
	
	OCR3A = bml;
	OCR3B = bmr;
}

int error()
{
	if(bit_is_clear(PINC, sen1) && bit_is_clear(PINC, sen2))
		err -3;
		
	else if(bit_is_clear(PINC, sen2) && bit_is_clear(PINC, sen3))
		err -1;
		
	else if(bit_is_clear(PINC, sen3) && bit_is_clear(PINC, sen4))
		err 1;
		
	else if(bit_is_clear(PINC, sen4) && bit_is_clear(PINC, sen5))
		err 3;
		
	else if(bit_is_clear(PINC, sen5))
		err 4;
	
	else if(bit_is_clear(PINC, sen1))
		err -4;
	
	else if(bit_is_clear(PINC, sen4))
		err 2;
	
	else if(bit_is_clear(PINC, sen3))
		err 0;
	
	else if(bit_is_clear(PINC, sen2))
		err -2;

	else
		return 0;
}

int main(void)
{
	int pullyu = false, pullyd = false, discl = false, discr = false;
    port_init();
	pwm_init();
	ps2_init();
//	lcd_init();
	
	joystick_config();
	joystick_config();
	joystick_config();

    for(;;){
        joystick_poll();
	/*	lcd_gotoxy1(0);
		lcd_showvalue(data0);
		lcd_string("   ");
		lcd_gotoxy2(0);
		lcd_showvalue(data1);
		lcd_string("   ");
		*/if(data0 == 0) {
			leftFwd(520);
			rightFwd(520);
			//lcd_gotoxy1(9);
			//lcd_string("FWD");
		}

		else if(data0 == 48) {
			leftRev(520);
			rightRev(520);
			//lcd_gotoxy1(9);
			//lcd_string("REv");
		}

		else if(data0 == 16) {
			rightFwd(400);
			leftRev(300);
			//lcd_gotoxy1(9);
			//lcd_string("LEF");
		}

		else if(data0 == 112) {
			leftFwd(400);
			rightRev(300);
			//lcd_gotoxy1(9);
			//lcd_string("RIG");
		}

		else if(data0 == 240) {
			Stop();
		}

		if(data1 == 31) {
			pullyu = true;
			OCR4A = 700;
			PORTG |= (1 << PG4);
		}

		else if(data1 == 127) {
			pullyd = true;
			OCR4A = 700;
			PORTG &= ~(1 << PG4);
		}

		else if(data1 == 255) {
			if(pullyu == true || pullyd == true) {
				pullyu = false;
				pullyd = false;
			}
			OCR4A = 0;
		}

		if(data1 == 15) {
			discr = true;
			OCR4B = 700;
			PORTL |= (1 << PL0);
		}

		else if(data1 == 63) {
			discl = true;
			OCR4B = 700;
			PORTL &= ~(1 << PL0);
		}

		else if(data1 == 255) {
			if(discr == true) {
				discr = false;
				OCR4B = 40;
				PORTL |= (1 << PL0);
			}
			else if(discl == true) {
				discl = false;
				OCR4B = 40;
				PORTL &= ~(1 << PL0);
			}
		}
		
		if(data1 == 135)
		{
			PORTL |= (1<<1) | (1<<0);
			PORTL &= ~(1<<2);
			PORTL &= ~(1<<3);
			pid();
			//lcd_gotoxy1(9);
			//lcd_string("PID");
		}
		
		else
		{
			Stop();
		}
    }
    return 0;   /* never reached */
}
