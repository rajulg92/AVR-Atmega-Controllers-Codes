//always call begin() function first before initializing ADXL345 in the main

#include "I2C\I2C_640.h"

#define DEVICE (0x53) // Device address as specified in data sheet 
int _buff[6];
int ADXL345_Xaxis;
int ADXL345_Yaxis;
int ADXL345_Zaxis;
char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1

void ADXL345_init(void);
void readAccel(void);
void writeTo(uint8_t address, uint8_t val);
void readFrom(uint8_t address, int num, uint8_t _buff[]);

void ADXL345_init(void)
{
	writeTo(DATA_FORMAT, 0x01);
	writeTo(POWER_CTL, 0x08);
}

void readAccel(void) 
{
	uint8_t howManyBytesToRead = 6;
	readFrom( DATAX0, howManyBytesToRead, _buff); //read the acceleration data from the ADXL345

	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	ADXL345_Xaxis = (((int)_buff[1]) << 8) | _buff[0];   
	ADXL345_Yaxis = (((int)_buff[3]) << 8) | _buff[2];
	ADXL345_Zaxis = (((int)_buff[5]) << 8) | _buff[4];
}

void writeTo(uint8_t address, uint8_t val) 
{
	beginTransmission(DEVICE); // start transmission to device 
	write(address);             // send register address
	write(val);                 // send value to write
	endTransmission();         // end transmission
}

void readFrom(uint8_t address, int num, uint8_t _buff[]) 
{
	beginTransmission(DEVICE); // start transmission to device 
	write(address);             // sends address to read from
	endTransmission();         // end transmission
	
	beginTransmission(DEVICE); // start transmission to device
	requestFrom(DEVICE, num);    // request 6 bytes from device
	
	
	int i = 0;
	while(available())         // device may send less than requested (abnormal)
	{ 
		_buff[i] = read();    // receive a byte
		i++;
	}
	endTransmission();         // end transmission
}