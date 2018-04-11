//PD7 Sensor output

#define F_CPU 8000000UL

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "DB18S20.h"
#include "LCD.h"


extern uint16_t therm_read_temperature(uint8_t DQ_pass);

char printbuff[10];

uint8_t DQ;

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
uint16_t therm_read_temperature( uint8_t DQ_pass )
{
	DQ=DQ_pass;
		
	// Buffer length must be at least 12bytes long! ["+XXX.XXXX C"]
	uint8_t temperature[2];
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
	therm_reset();
	
	float f_temp = ( (temperature[1] << 8) + temperature[0] )*0.0625;
	uint16_t i_temp= ceil(f_temp*10);
	
	return i_temp;
}



void main()
{

int16_t TES2=0;


DDRD=0X00;
lcd_init();
_delay_ms(10);

while(1)
{

	TES2 = therm_read_temperature(7);
	
	lcd_gotoxy1(0);
	lcd_showvalue(TES2);
	//lcd_gotoxy1(8);
	//lcd_showvalue(TES1>>8);

}

}