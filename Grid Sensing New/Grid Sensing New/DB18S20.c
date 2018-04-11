/*
 * DB18S20.c
 *
 * Created: 2014-08-09 12:25:04 PM
 *  Author: Samir
 */ 

#define F_CPU 8000000UL
#define CRC8INIT    0x00
#define CRC8POLY    0x18              // 0x18 = X^8+X^5+X^4+X^0

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <util/crc16.h>
#include "UART.h"
#include "DB18S20.h"

uint8_t DQ;
char printbuff[10];

void THERM_INPUT_MODE(uint8_t pin)
{
	THERM_DDR&=~(1<<pin);
}
void THERM_OUTPUT_MODE(uint8_t pin) 
{
	THERM_DDR|=(1<<pin);
}
void THERM_LOW(uint8_t pin) 
{
	THERM_PORT&=~(1<<pin);
}
void THERM_HIGH(uint8_t pin) 
{
	THERM_PORT|=(1<<pin);
}
uint8_t therm_reset()
{
	uint8_t i;
	//Pull line low and wait for 480uS
	THERM_LOW(DQ);
	THERM_OUTPUT_MODE(DQ);
	_delay_us(480);
	//Release line and wait for 60uS
	THERM_INPUT_MODE(DQ);
	_delay_us(60);
	//Store line value and wait until the completion of 480uS period
	i=(THERM_PIN & (1<<DQ));
	_delay_us(420);
	//Return the value read from the presence pulse (0=OK, 1=WRONG)
	return i;
}
void therm_write_bit(uint8_t bit){
	//Pull line low for 1uS
	THERM_LOW(DQ);
	THERM_OUTPUT_MODE(DQ);
	_delay_us(1);
	//If we want to write 1, release the line (if not will keep low)
	if(bit) THERM_INPUT_MODE(DQ);
	//Wait for 60uS and release the line
	_delay_us(60);
	THERM_INPUT_MODE(DQ);
}
uint8_t therm_read_bit(void){
	uint8_t bit=0;
	//Pull line low for 1uS
	THERM_LOW(DQ);
	THERM_OUTPUT_MODE(DQ);
	_delay_us(1);
	//Release line and wait for 14uS
	THERM_INPUT_MODE(DQ);
	_delay_us(14);
	//Read line value
	if(THERM_PIN&(1<<DQ)) bit=1;
	//Wait for 45uS to end and return read value
	_delay_us(45);
	return bit;
}
uint8_t therm_read_byte(void){
	uint8_t i=8, n=0;
	while(i--){
		//Shift one position right and store read value
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}
void therm_write_byte(uint8_t byte){
	uint8_t i=8;
	while(i--){
		//Write actual bit and shift one position right to make	the next bit ready
		therm_write_bit(byte&1);
		byte>>=1;
	}
}
uint8_t crc8( uint8_t *data, uint16_t number_of_bytes_in_data )
{
	uint8_t  crc;
	uint16_t loop_count;
	uint8_t  bit_counter;
	uint8_t  b;
	uint8_t  feedback_bit;
	
	crc = CRC8INIT;

	for (loop_count = 0; loop_count != number_of_bytes_in_data; loop_count++)
	{
		b = data[loop_count];
		
		bit_counter = 8;
		do {
			feedback_bit = (crc ^ b) & 0x01;
			
			if ( feedback_bit == 0x01 ) {
				crc = crc ^ CRC8POLY;
			}
			crc = (crc >> 1) & 0x7F;
			if ( feedback_bit == 0x01 ) {
				crc = crc | 0x80;
			}
			
			b = b >> 1;
			bit_counter--;
			
		} while (bit_counter > 0);
	}
	
	return crc;
}
int16_t therm_read_temperature(uint8_t DQ_pass, int16_t *tes)
{
	
	DQ=DQ_pass;
		
	// Buffer length must be at least 12bytes long! ["+XXX.XXXX C"]
	uint8_t temperature[9];
	//Reset, skip ROM and start temperature conversion
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CONVERTTEMP);
	//Wait until conversion is complete
	while(!therm_read_bit());
	//Reset, skip ROM and send command to read Scratchpad
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);
	
	//Read Scratchpad (only 2 first bytes)
	temperature[0]=therm_read_byte();
	temperature[1]=therm_read_byte();	
	temperature[2]=therm_read_byte();
	temperature[3]=therm_read_byte();
	temperature[4]=therm_read_byte();
	temperature[5]=therm_read_byte();
	temperature[6]=therm_read_byte();
	temperature[7]=therm_read_byte();
	temperature[8]=therm_read_byte();
	
	therm_reset();
	
	uint8_t crc = 0x00;
	uint8_t b=0;
	
	for (b=0; b<8; b++) 
	{
		crc = _crc_ibutton_update(crc, temperature[b]);
	}
	
	if (temperature[8] == crc)
	{
		float f_temp = ( (temperature[1] << 8) + temperature[0] )*0.0625;
		*tes = ceil(f_temp*10);
		
		return 0;
	} 
	else
	{
		return -1;
	}
}
