/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include "joystick.h"

#define sen1 6		//left extreme
#define sen2 5
#define sen3 2
#define sen4 3
#define sen5 4		//right extreme

#define true 1
#define false 0
#define mid 2
#define r 3
#define rm 4
#define l 5
#define lm 6

uint8_t data = 0, err = 0, acc_err = 0, inst_err = 0, prev_err = 0;
float p = 50, i = 0.01, d = 0.5, corr = 0;

void port_init()
{
	DDRE |= (1 << PE3) | (1 << PE4);
	DDRH |= (1 << PH3) | (1 << PH4);
	DDRJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3);
	DDRG |= (1 << PG4);
	DDRL |= (1 << PL0);
	DDRC = 0x00;
	PORTC = 0xFF;
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


	inst_err = err - prev_err;
	corr = p*err + i*acc_err + d*inst_err;
	acc_err += err;
	prev_err = err;

	OCR3A = 300 - ceil(corr);
	OCR3B = 300 + ceil(corr);
}
/*PC2 = err 0
  PC3 = err 1
  PC4 = err 2
  PC5 = err -1
  PC6 = err -2
*/

int main(void)
{
	int pullyu = false, pullyd = false, discl = false, discr = false, pidf = false;
    port_init();
	pwm_init();
	ps2_init();

	joystick_config();
	joystick_config();
	joystick_config();

    for(;;){
        joystick_poll();
		if(data0 == 0) {
			leftFwd(600);
			rightFwd(600);
		}

		else if(data0 == 48) {
			leftRev(600);
			rightRev(600);
		}

		else if(data0 == 16) {
			rightFwd(400);
			leftRev(300);
		}

		else if(data0 == 112) {
			leftFwd(400);
			rightRev(300);
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

		else if(data1 == 135) {
			pidf = true;
			PORTJ |= (1 << PJ1) | (1 << PJ2);
			PORTJ &= ~(1 << PJ0);
			PORTJ &= ~(1 << PJ3);
			pid();
		}
		
		else if(data1 == 255) {
			if(pidf == true) {
				pidf = false;
				Stop();
			}
		}

		else if(data1 == 255) {
			if(discr == true) {
				discr = false;
				OCR4B = 40;
				PORTL &= ~(1 << PL0);
			}
			else if(discl == true) {
				discl = false;
				OCR4B = 40;
				PORTL &= ~(1 << PL0);
			}
		}
    }
    return 0;   /* never reached */
}
