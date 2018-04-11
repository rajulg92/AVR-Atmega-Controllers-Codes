/*
DHT Library 0x03

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.

References:
  - DHT-11 Library, by Charalampos Andrianakis on 18/12/11
*/


#ifndef DHT_H_
#define DHT_H_

#include <stdio.h>
#include <avr/io.h>

//setup port
#define DHT_DDR DDRA
#define DHT_PORT PORTA
#define DHT_PIN PINA

//sensor type
#define DHT_DHT11 1
#define DHT_DHT22 2
#define DHT_TYPE DHT_DHT22

//enable decimal precision (float)

#define DHT_FLOAT 1

//timeout retries
#define DHT_TIMEOUT 200

//functions
extern int8_t dht_gettemperaturehumidity(int16_t *t1,uint16_t *h1, uint8_t pin);

#endif
