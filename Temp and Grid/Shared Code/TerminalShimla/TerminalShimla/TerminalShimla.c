/*
 * HMI.c
 *
 * Created: 05-07-2015
 * Author: Samir
 */ 

#define F_CPU 8000000UL
#define BAUD 9600

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dht.h"
#include "DB18S20.h"
#include "UART.h"

uint8_t i=0,j=0;
uint16_t t=0;
char ch1[4],ch2[4],ch3[4],ch4[4],ch5[4],ch6[4],ch7[4],ch8[4],ch9[4];

int main(void)
{
	    DDRC = 0b00000000;
	    DDRD = 0b00000010;
		DDRB = 0b00000000;
	    
	    PORTC = 0x00;
	    PORTD = 0x00;
		PORTB = 0x00;
	    
		UART_init();
		sei();
		
		_delay_ms(2000);

		int16_t  t_room=0,t_ambi=0,TES1=0,TES2=0,Pipe=0,Plate_1=0,Plate_2=0;
		uint16_t h_room=0,h_ambi=0;
		uint8_t D=0,W=0,G=0;
		//char ch1[4],ch2[4],ch3[4],ch4[4];//,ch5[4],ch6[4];
	
    while(1)
    {	

			// Room Temperature and Humidity
			if(dht_gettemperaturehumidity(&t_room, &h_room, PD4) != -1)
			{
			
			}			

			// Ambient temperature and Humidity 		
			if(dht_gettemperaturehumidity(&t_ambi, &h_ambi, PD3) != -1)
			{
			
			}
		
			
					
			// TES 1 Temperature PA3
			TES1 = therm_read_temperature(PC3);
		
			// TES 2 Temperature PA0
			TES2 = therm_read_temperature(PC1);
		
			// Pipe Temperature PC0
			Pipe = therm_read_temperature(PC0);
			
			// Pipe Temperature PC0
			Plate_1 = therm_read_temperature(PC4);
			
			// Pipe Temperature PC0
			Plate_2 = therm_read_temperature(PC5);
				
		// Door Sensor
		if( PINC & (1<<PINC5) )
		{
			D=0;
			//UART_puts("Water = 0 \r\n");
			}else{
			D=10;
			//UART_puts("Water = 1 \r\n");
		}
			
		// Water Sensor
		if( PINC & (1<<PINC4) )
		{
			W=0;
			//UART_puts("Door = 0 \r\n");	
		}else{
			W=10;
			//UART_puts("Door = 1 \r\n");
		}
		
		// Grid Sensing
		if( (PIND & (1<<PIND6)) && (PIND & (1<<PIND7) && (PINB & (1<<PINB0))))
		{
			G=10;
			//UART_puts("Grid = 1 \r\n\r\n");
		}else{
			G=0;
			//UART_puts("Grid = 0  \r\n\r\n");
		}		
		
		/*
		itoa(t_room,ch1,10);
		itoa(h_room,ch2,10);
		itoa(t_ambi,ch3,10);
		itoa(h_ambi,ch4,10);
		itoa(TES1,ch5,10);
		itoa(TES2,ch6,10);
		itoa(D,ch7,10);
		itoa(G,ch8,10);
		itoa(W,ch9,10);

		UART_puts("Room Temperature		: "); UART_puts(ch1);  UART_puts("\r\n");
		UART_puts("Room Humidity		: "); UART_puts(ch2);  UART_puts("\r\n");
		UART_puts("Ambient Temperature	: "); UART_puts(ch3);  UART_puts("\r\n");
		UART_puts("Ambient Humidity		: "); UART_puts(ch4);  UART_puts("\r\n");
		UART_puts("TES 1				: "); UART_puts(ch5);  UART_puts("\r\n");
		UART_puts("TES 2				: "); UART_puts(ch6);  UART_puts("\r\n");
		UART_puts("Door					: "); UART_puts(ch7);  UART_puts("\r\n");
		UART_puts("Grid					: "); UART_puts(ch8);  UART_puts("\r\n");
		UART_puts("Water Level			: "); UART_puts(ch9);  UART_puts("\r\n");
		 _delay_ms(2000);
		*/

		UART_putc(0xFF);
		UART_putc(0xFE);
		UART_putc(0xFD);
		UART_putc(t_room);		//0
		UART_putc(t_room>>8);	//1
		UART_putc(h_room);		//2
		UART_putc(h_room>>8);	//3
		UART_putc(t_ambi);		//4
		UART_putc(t_ambi>>8);	//5
		UART_putc(Pipe);		//6
		UART_putc(Pipe>>8);		//7
		UART_putc(TES1);		//8
		UART_putc(TES1>>8);		//9
		UART_putc(TES2);		//10
		UART_putc(TES2>>8);		//11
		UART_putc(Plate_1);		//12
		UART_putc(Plate_1>>8);	//13
		UART_putc(G);			//14
		UART_putc(0x00);		//15
		UART_putc(Plate_2);		//16		
		UART_putc(Plate_2>>8);	//17
		 //_delay_ms(500);
		
	}
	
	return(0);
}
