/*
DHT Library 0x03

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#define F_CPU 8000000UL

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dht.h"

/*
 * get data from sensor
 */

uint8_t DHT_INPUTPIN=0;


int8_t dht_getdata(int16_t *t1, uint16_t *h1) {

	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	//reset port
	DHT_DDR |= (1<<DHT_INPUTPIN); //output
	DHT_PORT |= (1<<DHT_INPUTPIN); //high
	_delay_ms(100);

	//send request
	DHT_PORT &= ~(1<<DHT_INPUTPIN); //low
	_delay_us(500);
	
	DHT_PORT |= (1<<DHT_INPUTPIN); //high
	DHT_DDR &= ~(1<<DHT_INPUTPIN); //input
	_delay_us(40);

	//check start condition 1
	if((DHT_PIN & (1<<DHT_INPUTPIN))) {
		return -1;
	}
	_delay_us(80);
	//check start condition 2
	if(!(DHT_PIN & (1<<DHT_INPUTPIN))) {
		return -1;
	}
	_delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!(DHT_PIN & (1<<DHT_INPUTPIN))) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
			_delay_us(30);
			if(DHT_PIN & (1<<DHT_INPUTPIN)) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			timeoutcounter = 0;
			while(DHT_PIN & (1<<DHT_INPUTPIN)) { //wait until input get low (non blocking)
				timeoutcounter++;
				if(timeoutcounter > DHT_TIMEOUT) {
					return -1; //timeout
				}
			}
		}
		bits[j] = result;
	}

	//reset port
	DHT_DDR |= (1<<DHT_INPUTPIN); //output
	DHT_PORT |= (1<<DHT_INPUTPIN); //low
	_delay_ms(100);

	//check checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity
		 *h1 = bits[0]<<8 | bits[1];
		 *t1 = bits[2]<<8 | bits[3];
		
		return 0;
	}

	return -1;
}


/*
 * get temperature and humidity
 */
int8_t dht_gettemperaturehumidity(int16_t *t1, uint16_t *h1, uint8_t pin) {

	DHT_INPUTPIN=pin;
	return dht_getdata(t1,h1);
}


