/*
 * DC_Sensing.c
 *
 * Created: 2015-05-31 1:20:13 PM
 *  Author: Samir
 */ 


#define F_CPU 8000000UL
#define BAUD 9600
#define dcountterminal 19

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "UART.h"

uint8_t header[3]={0,0,0},i=0,j=0,data,refdata[dcountterminal]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},refcounter=1,step1=0,step2=0,step3=0,takedata=0;
uint16_t t=0;

char ch1[4],ch2[4],ch3[6],ch4[6],ch5[4],ch6[4],ch7[6],ch8[6],ch9[4],ch10[4],ch11[6],ch12[6],ch13[4],ch14[4],ch15[6],ch16[6];

uint16_t filter(uint16_t array[100])
{
	for(i=1;i<100;i++)
	{
		for(j=0;j<100-i;j++)
		{
			if(array[j]>=array[j+1])
			{
				t=array[j];
				array[j]=array[j+1];
				array[j+1]=t;
			}
		}
	}
	
	uint16_t result = ceil(( array[45] + array[46] + array[47] + array[48] + array[49] + array[50] + array[51] + array[52] + array[53] + array[54] ) /10);
	return result;
}

int main(void)
{

	//Pre-scalar /8, Interrupt Disable, Auto Trigger off,
	ADCSRA = 0b10000111;
	DDRD = 0b00000010;
	PORTD = 0x00;
	
	DDRB  = 0x00;

	uint16_t filter_array[100];
	uint16_t v2=0,v4=0,v8=0,v3=0,v7=0;
	uint16_t b_i_u=0,b_v_u=0,s1=0,s2=0,s3=0,s_i_u=0,s_v_u=0,frq_u=0;
	float b_v=0.0,b_i=0.0,s_v=0.0,s_i=0.0,frq=0.0;
	uint16_t TH=0,TAB=0,RH=0,RAB=0;
	
	UART_init();

	_delay_ms(100);
	
	while (1)
	{
		cli();
		//Battery Voltage
		ADMUX  = 0b01000001;
		for (i=0;i<100;i++)
		{
			_delay_ms(2);
			ADCSRA |= (1<<ADSC);
			while(!(ADCSRA & (1<<ADIF)));
			ADCSRA|=(1<<ADIF);
			filter_array[i] = ADC;
		}
		v2 = filter(filter_array);
		
		//Solar String Current
		ADMUX  = 0b01000010;
		for (i=0;i<100;i++)
		{
			_delay_ms(1);
			ADCSRA |= (1<<ADSC);
			while(!(ADCSRA & (1<<ADIF)));
			ADCSRA|=(1<<ADIF);
			filter_array[i] = ADC;
		}
		v3 = filter(filter_array);
		
		//Battery Current
		ADMUX  = 0b01000011;
		for (i=0;i<100;i++)
		{
			_delay_ms(1);
			ADCSRA |= (1<<ADSC);
			while(!(ADCSRA & (1<<ADIF)));
			ADCSRA|=(1<<ADIF);
			filter_array[i] = ADC;
		}
		v4 = filter(filter_array);
		
		// Frequency
		ADMUX  = 0b01000110;
		for (i=0;i<100;i++)
		{
			_delay_ms(2);
			ADCSRA |= (1<<ADSC);
			while(!(ADCSRA & (1<<ADIF)));
			ADCSRA|=(1<<ADIF);
			filter_array[i] = ADC;
		}
		v7 = filter(filter_array);
		
		// Solar Voltage
		ADMUX  = 0b01000111;
		for (i=0;i<100;i++)
		{
			_delay_ms(2);
			ADCSRA |= (1<<ADSC);
			while(!(ADCSRA & (1<<ADIF)));
			ADCSRA|=(1<<ADIF);
			filter_array[i] = ADC;
		}
		v8 = filter(filter_array);
		
		if (PINB & (1<<PINB0)){s1 = 10;} else{s1 = 0;}
		if (PINB & (1<<PINB1)){s2 = 10;} else{s2 = 0;}
		if (PINB & (1<<PINB2)){s3 = 10;} else{s3 = 0;}
		
		s_v = ((float)v8*0.9)-80.05;
		b_v = (float)v2*0.0293;
	    frq = ((float)v7*0.085)+2.0;
	
		b_i = ((float)v4*0.185)-92.35;
		s_i= ((float)v3*0.185)-92.35;
		b_i_u = (uint16_t)ceil(b_i*10)+100;
		b_v_u = (uint16_t)ceil(b_v*10);
		s_i_u= (uint16_t)ceil(s_i*10)+100;
		s_v_u= (uint16_t)ceil(s_v*10); 
		frq_u= (uint16_t)ceil(frq*10);
		
		/*
		TH =256*refdata[2]+refdata[1];
		TAB=256*refdata[6]+refdata[5];
		RH =256*refdata[4]+refdata[3];
		RAB=256*refdata[8]+refdata[7];
		*/
		
		UART_putc(0x01);		// 0x0001
		UART_putc(0x00);	
		UART_putc(s_v_u);		// Solar Voltage
		UART_putc(s_v_u>>8);
		UART_putc(b_v_u);		// Battery Voltage
		UART_putc(b_v_u>>8);
		UART_putc(b_i_u);		// Load Current
		UART_putc(b_i_u>>8);
		UART_putc(s_i_u);		// Solar Current
		UART_putc(s_i_u>>8);		
		UART_putc(frq_u);		// 0xFFFF
		UART_putc(frq_u>>8);
		//UART_putc(0xFF);		// 0xFFFF
		//UART_putc(0xFF);
		UART_putc(0xFF);		// 0xFFFF
		UART_putc(0xFF);
		UART_putc(0xFF);		// 0xFFFF
		UART_putc(0xFF);
		
		_delay_ms(500);
		
		UART_putc(0x02);		// 0x0002
		UART_putc(0x00);
		UART_putc(s1);			// HPS
		UART_putc(s1>>8);
		UART_putc(s2);			// LPS
		UART_putc(s2>>8);
		UART_putc(s3);			// INT69
		UART_putc(s3>>8);
		UART_putc(refdata[14]);	// Grid
		UART_putc(refdata[15]);
		UART_putc(refdata[12]);	// Door
		UART_putc(refdata[13]);
		UART_putc(refdata[16]);	// Water Level
		UART_putc(refdata[17]);	
		UART_putc(0xFF);		// 0xFFFF
		UART_putc(0xFF);
		
		_delay_ms(500);
		
		UART_putc(0x03);		// 0x0003
		UART_putc(0x00);
		UART_putc(refdata[1]);	// TH1
		UART_putc(refdata[2]);	
		UART_putc(refdata[5]);	// TA
		UART_putc(refdata[6]);
		UART_putc(refdata[9]);	// TES1
		UART_putc(refdata[10]);
		UART_putc(refdata[11]);	// TES2
		UART_putc(refdata[12]);
		UART_putc(refdata[3]);	// RH1
		UART_putc(refdata[4]);
		UART_putc(refdata[7]);	// RA
		UART_putc(refdata[8]);
		UART_putc(0xFF);		// 0xFFFF
		UART_putc(0xFF);

		/*
		itoa( s_v_u,ch1,10);
		itoa( b_v_u,ch2,10);
		itoa( s_i_u,ch3,10);
		itoa( b_i_u,ch4,10);
		itoa( s1,ch5,10);
		itoa( s2,ch6,10);
		itoa( s3,ch7,10);
		itoa( refdata[13],ch8,10);
		itoa( refdata[14],ch9,10);
		itoa( refdata[15],ch10,10);
		itoa( TH,ch11,10);
		itoa( TAB,ch12,10);
		itoa( RH,ch13,10);
		itoa( RAB,ch14,10);
		
		UART_puts("Solar Voltage		:");UART_puts(ch1);UART_puts("\r\n");
		UART_puts("Battery Voltage		:");UART_puts(ch2);UART_puts("\r\n");
		UART_puts("Solar Current		:");UART_puts(ch3);UART_puts("\r\n");
		UART_puts("Battery Current		:");UART_puts(ch4);UART_puts("\r\n");
		UART_puts("HPS Fault			:");UART_puts(ch5);UART_puts("\r\n");
		UART_puts("LPS Fault			:");UART_puts(ch6);UART_puts("\r\n");
		UART_puts("INT69 Fault			:");UART_puts(ch7);UART_puts("\r\n");
		UART_puts("Door 				:");UART_puts(ch8);UART_puts("\r\n");
		UART_puts("Grid					:");UART_puts(ch9);UART_puts("\r\n");
		UART_puts("Water Level			:");UART_puts(ch10);UART_puts("\r\n");
		UART_puts("Room Temperature		:");UART_puts(ch11);UART_puts("\r\n");
		UART_puts("Ambient Temperature	:");UART_puts(ch12);UART_puts("\r\n");
		UART_puts("Room Humidity		:");UART_puts(ch13);UART_puts("\r\n");
		UART_puts("Ambient Humidity     :");UART_puts(ch14);UART_puts("\r\n");
		*/
		sei();
		_delay_ms(3000);
		cli();
	}
	return(0);
}

ISR(USART_RXC_vect)
{
	
		data= UDR;
		header[0]=header[1];
		header[1]=header[2];
		header[2]=data;
		
		if (takedata==1)
		{
			refdata[refcounter]=data;
			refcounter++;
		}
		
		if ((header[0]==0xFF)&&(header[1]==0xFE)&&(header[2]==0xFD))
		{
			//UART_putc('S');
			refcounter=1;
			takedata=1;
		}
		
		if (refcounter>=19)
		{
			takedata=0;
		}
		
	/*	
		if (step3 ==1)
		{
			refdata[refcounter]=data;
			refcounter++;
		}
		
		if ( (data == 0xFF) && (step2 == 0) && (step3 == 0))
		{
			step1 = 1;
		}
		if ( (data == 0xFE) && (step1 == 1) && (step3 == 0))
		{
			step2 = 1;
		}
		
		if ( (data == 0xFD) && (step2 == 1) && (step1 == 1))
		{
			step3 = 1;step1=0;step2=0;
			refcounter = 1;
		}
		
		if (refcounter == 15)
		{
			step1 =0;step2=0;step3=0;
		}
		
	*/
	/*
	if (step3 ==1)
	{
		refdata[refcounter]=data;
		refcounter++;
	}
	if (data == 0xFF && step3 ==0)
	{
		step1=1;
	}
	if (data == 0xFE && step1==1)
	{
		step1=0;
		step2=1;
	}
	if (data == 0xFD && step2==1)
	{
		step1=0;
		step2=0;
		step3 =1;
		refcounter =1;
	}
	
	if (refcounter ==16)
	{
		step1=0;step2=0;step3=0;
	}
	*/
}

