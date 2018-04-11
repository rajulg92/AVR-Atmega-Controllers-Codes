/*The useless stuff for Rajul
  How we will be implementing
  See for the robot to move in the straight line the speed of both the wheels should be 
  constant...Which we are giving by keeping the OCR same for both the wheels
  So, first we will try to move it dead straight line 

  For the curve tracing part I am thinking of sampling the encoder values at minute instants of timw and storing them in the 
  array then check the current error against the previously stored value...If the error is not equal to zero then we take the 	corrective action using our PID algorithm
  So, we require a timer to sample the values and also to check them at various instants
  Next we require a array to keep track of the error values at different instants of time
  Then we need the usual PID stuff and the obtain the correction factor and take the corresponding factor 
  The only problem I see here is the slippage 
  Feel free to add anything
 */
//This is the code for the straight line

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

void port_init()
{
	DDRA |= (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4);
	DDRH |= (1 << PH3) | (1 << PH4) | (1 << PH5);
}

void pwm_init()
{
	TCCR4A = 0xA8;
	TCCR4B = 0x11;
}

int main()
{
	port_init();
	pwm_init();

	//enc1 is connected to right wheel
	uint16_t enc1, enc2; //values from the two encoders
	//for straight line this should be zero if I am guaranteed that they are moving with the same speed
	double kp = 0, ki = 0, kd = 0;
	int8_t acc_err = 0, prev_err = 0, inst_err;
	ICR = 737;
	OCR4A = 300;
	OCR4B = 300;
	
	while(1) {
		//to write the code for reception from two encoders

		uint16_t diff = enc1-enc2;
		inst_err = diff - prev_err;
		//if the difference is positive then the right wheel is moving faster than the left wheel
		//if the difference is negative then left wheel is moving faster than the right wheel
		//I need to balance the two

		bal = kp*diff + ki*acc_err + kd*inst_err;
		acc_err += diff;
		prev_err = inst_err;
		
		OCR4A = OCR4A - ceil(bal);
		OCR4B = OCR4B + ceil(bal);
	}
	return 0;
}
