//Master

#include<avr/io.h>
#include<util/delay.h>
//#include "LCD_640.h"
uint8_t recv_data;

void TWI_init_master(void) // Function to initialize master
{
	TWBR=0x01; // Bit rate
	TWSR&=~(1<<TWPS1); // Setting prescalar bits
	TWSR&=~(1<<TWPS0);
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
}

void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
	while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgement
}

void TWI_repeated_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
}

void TWI_write_address(unsigned char data)
{
	TWDR=data; // Address and write instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8)!= 0x18);  // Check for the acknoledgement
}	

void TWI_write_data(unsigned char data)
{
	TWDR=data; // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x28); // Check for the acknoledgement
}

void TWI_read_address(unsigned char data)
{
	TWDR=data; // Address and read instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknoledgement
}

uint8_t TWI_read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
	recv_data=TWDR;
	return recv_data;
}

void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void I2C_Write(uint8_t address, uint8_t data)
{
	TWI_start();
	TWI_write_address(address);
	TWI_write_data(data);
	TWI_stop();
}

uint8_t I2C_Read(uint8_t address)
{	
	uint8_t data_from_slave;
	TWI_start(); 
	TWI_read_address(address+1); // Function to write address and data direction bit(read) on SDA
	data_from_slave = TWI_read_data(); // Function to read data from slave 
	TWI_stop();
	return data_from_slave;
}

int main(void)
{
	_delay_ms(2000);
	//lcd_init();
	DDRB = 0xFF;
	
	TWI_init_master();  // Function to initialize TWI
	int data = 50;
	while(1)
	{
		//PORTB = I2C_Read(0x20);
		
	//	_delay_ms(500);
	}


}