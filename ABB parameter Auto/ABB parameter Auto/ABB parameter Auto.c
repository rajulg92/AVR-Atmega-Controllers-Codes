/*
 * ABB_Parameter_Set.c
 *
 * Created: 03-12-2015 12:13:36
 *  Author: RajulGupta
 */ 
//Fuse Bits E2 D9
#define  F_CPU 8000000UL
#define read  8
#define write 8

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART_0.h"
#include "UART_1.h"
#include "lcd_lib.h"

uint8_t crc_correct=0,response[8];
uint16_t CRC16_2(uint8_t *buf, uint8_t len);
void Send_Command(uint8_t *cmd, uint16_t *value, uint8_t read_count);


int main(void)
{
	// To stabilize the Oscillator
	
	_delay_ms(100);
	DDRD  = 0x0A;
	PORTD = 0xFF ;
	DIDR0= 0xFF;	

	// InItilize UART Baud=9600,8 bit, 1 Stop bit, No Parity
	UART_0_Init();
	UART_1_Init();
	LCDinit();
	LCDcursorOFF();
		
	LCDclr; // Clear LCD
	LCDhome();
	_delay_ms(2);
	LCDGotoXY(0,0);
	LCDstring("   Inserting",12);
	_delay_ms(2);
	LCDGotoXY(0,1);
	LCDstring("    Parameters..",16);
	
	_delay_ms(2000);
	
	//Define variables
	uint16_t Parameter;	
	//Parameter Write Commands
	//uint8_t Par1103[]={0x01,0x06,0x04,0x4E,0x00,0x08};//Ref1 Select 8
	uint8_t Par1601[]={0x01,0x06,0x06,0x40,0x00,0x07};//Run Enable 1
	uint8_t Par1604[]={0x01,0x06,0x06,0x43,0x00,0x08};//Fault Reset 8
	uint8_t Par2003[]={0x01,0x06,0x07,0xD2,0x00,0xE1};//Max allowed Motor Current 22.5
	uint8_t Par2201[]={0x01,0x06,0x08,0x98,0x00,0x00};//slope Profile 0
	uint8_t Par2202[]={0x01,0x06,0x08,0x99,0x00,0x0A};//Acceleration Time 1s
	uint8_t Par2203[]={0x01,0x06,0x08,0x9A,0x00,0x00};//Deceleration Time 0s
	uint8_t Par2204[]={0x01,0x06,0x08,0x9B,0x00,0x00};//Ramp Shape1 0
	uint8_t Par6005[]={0x01,0x06,0x17,0x74,0x03,0x20};//OverVoltage 800
	//uint8_t Par9904[]={0x01,0x06,0x26,0xAF,0x00,0x03};//Motor Control 3
	uint8_t Par9905[]={0x01,0x06,0x26,0xB0,0x00,0xE6};//Max. Output Voltage 230
	uint8_t Par9906[]={0x01,0x06,0x26,0xB1,0x00,0x69};//Output Current 10.5
	uint8_t Par9908[]={0x01,0x06,0x26,0xB3,0x0A,0xF0};//RPM 2800
	uint8_t Par9909[]={0x01,0x06,0x26,0xB4,0x00,0x17};//Power 2.3
	uint8_t Par2605[]={0x01,0x06,0x0A,0x2C,0x00,0x01};//???
	uint8_t Par1603[]={0x01,0x06,0x06,0x42,0x01,0x66};//Lock Key 358
	uint8_t Par1602[]={0x01,0x06,0x06,0x41,0x00,0x00};//Parameters Locked
	
	
	_delay_ms(20);
	
	//Send_Command(&Par1103,&Parameter,read);
	Send_Command(&Par1601,&Parameter,read);
	Send_Command(&Par1604,&Parameter,read);
	Send_Command(&Par2003,&Parameter,read);
	Send_Command(&Par2201,&Parameter,read);
	Send_Command(&Par2202,&Parameter,read);
	Send_Command(&Par2203,&Parameter,read);
	Send_Command(&Par2204,&Parameter,read);
	Send_Command(&Par6005,&Parameter,read);
	Send_Command(&Par9905,&Parameter,read);
	Send_Command(&Par9906,&Parameter,read);
	Send_Command(&Par9908,&Parameter,read);
	Send_Command(&Par9909,&Parameter,read);
	Send_Command(&Par2605,&Parameter,read);
	Send_Command(&Par1603,&Parameter,read);
	Send_Command(&Par1602,&Parameter,read);
		
	_delay_ms(20);
	
	LCDinit();LCDclr();LCDhome();
	LCDGotoXY(0,0);LCDstring("  Parameters",12);
	LCDGotoXY(0,1);LCDstring("  Updated.....",14);
	
	_delay_ms(10000);
	
}


void Send_Command(uint8_t* cmd, uint16_t* value, uint8_t read_count)
{
	uint8_t rx_try=9;
	uint16_t crc_sent=0, crc_received=0, crc_calculated=0;
	crc_sent = CRC16_2(cmd,6);
	while (rx_try>0) 
	{
		UART_0_Putc(cmd[0]);
		UART_0_Putc(cmd[1]);
		UART_0_Putc(cmd[2]);
		UART_0_Putc(cmd[3]);
		UART_0_Putc(cmd[4]);
		UART_0_Putc(cmd[5]);
		UART_0_Putc(crc_sent);
		UART_0_Putc(crc_sent>>8);

		//Response Read
		for( uint8_t read_byte=0; read_byte<read_count; read_byte++)
		{
			while ( !(UCSR0A & (1<<RXC0)) )
			;
			response[read_byte] = UDR0;
		}
		
		if (read_count == read)
		{
			crc_received = (response[7]<<8) | response[6];
			crc_calculated = CRC16_2(&response,6);
		} 
		else
		{
			crc_received = (response[7]<<8) | response[6];
			crc_calculated = CRC16_2(&response,6);	
		}
		
		if ( crc_calculated == crc_received)
		{
			break;
		}
		
		rx_try--;
			
	} 
	
	if (read_count == read)
	{
		*value = response[5]+response[4]*256;		
	} 
}

uint16_t CRC16_2(uint8_t* buf, uint8_t len)
{
	uint16_t crc = 0xFFFF;

	for (uint8_t pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

		for (int i = 8; i != 0; i--) {		// Loop over each bit
			if ((crc & 0x0001) != 0) {      // If the LSB is set
				crc >>= 1;                  // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else                            // Else LSB is not set
			crc >>= 1;						// Just shift right
		}
	}
	// Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
	return crc;
}

