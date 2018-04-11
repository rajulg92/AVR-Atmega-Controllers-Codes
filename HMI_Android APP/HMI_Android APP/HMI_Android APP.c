/*
 * HMI_Android_APP.c
 *
 * Created: 24-12-2015 12:51:32
 *  Author: RajulGupta
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define delay1 2500
#define delay2 1000

#define SHIFTPORT PORTC
#define SHTP PC2
#define STCP PC1
#define DS PC0

#define LED_PORT    PORTA
#define GATE_PORT   PORTB

#define LED_GREEN   PA0
#define LED_ORANGE PA1
#define LED_RED PA2
#define LED_TEMP PA3
#define LED_HUM PA4
#define LED_WATT PA5

#define RE PD2
#define DE PD3

#include <avr/io.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>

long int i=0;
uint8_t t=0,h=0,p=0,crp=0,crp2=0,mode=0,power=0,tm=0,header[3]={0,0,0},header1[3]={0,0,0},data,data1,refdata[6]={0,0,0,16,88,18},refdata1[3]={0,0,0},refcounter=0,refcounter1=0,takedata=0,takedata1=0;
uint8_t eeprom_wr=13; //Because it initializes to 1 sporadically when defined in the previous line.. compiler error?

char Temp[2],Hum[2],Pow[2],Tmptr[2],Hmdty[2],Pwr[2];

int display(char ch);

void shift_byte(unsigned char x, unsigned char y, unsigned char z)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if((x & (1<<i))== 0) cbi(PORTC,DS);
		else sbi(PORTC,DS);
		sbi(PORTC,SHTP);
		cbi(PORTC,SHTP);
	}
	
	for(i=0;i<8;i++)
	{
		if((y & (1<<i))== 0) cbi(PORTC,DS);
		else sbi(PORTC,DS);
		sbi(PORTC,SHTP);
		cbi(PORTC,SHTP);
	}
	
	for(i=0;i<8;i++)
	{
		if((z & (1<<i))== 0) cbi(PORTC,DS);
		else sbi(PORTC,DS);
		sbi(PORTC,SHTP);
		cbi(PORTC,SHTP);
	}
	
	sbi(PORTC,STCP);
	cbi(PORTC,STCP);
	_delay_ms(100);
}

void timer1_init()
{
	TCNT1=56165;
	//TIMSK= (1<<TOIE1);
	TCCR1B |= (1 << CS12);
}

void uart_0_init(unsigned int ubrr2)
{
	UCSR0B = 0x00;
	UCSR0A = 0x00;
	UCSR0C = (1 << URSEL0) | 0x06;
	UBRR0H = (unsigned char)(ubrr2>>8);
	UBRR0L = (unsigned char)ubrr2;
	UCSR0B = 0x98;
}

void UART_0_putc(unsigned char data0)
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data0;
}

void uart_1_init(unsigned int ubrr2)
{
	UCSR1B = 0x00;
	UCSR1A = 0x00;
	UCSR1C = (1 << URSEL1) | 0x06;
	UBRR1H = (unsigned char)(ubrr2>>8);
	UBRR1L = (unsigned char)ubrr2;
	UCSR1B = 0x98;
}

void UART_1_putc(unsigned char datas)
{
	while ( !(UCSR1A & (1<<UDRE1)) );
	UDR1 = datas;
}

int main(void)
{
	cli();

	CLKPR = (1<<CLKPCE);
	CLKPR = 0x00;
	MCUCSR |= (1<<JTD);

	DDRA = 0b11111111;
	DDRB = 0b11111011;
	DDRC = 0b11111111;
	DDRD = 0b11111110;
	
	timer1_init();
	cbi(PORTD,RE);
	uart_0_init(MYUBRR);
	uart_1_init(MYUBRR);
	
	_delay_ms(10);
	crp   = eeprom_read_byte((uint8_t*)16);
	crp2=crp;
	mode  = eeprom_read_byte((uint8_t*)17);
	power = eeprom_read_byte((uint8_t*)18);
	_delay_ms(10);

	
	PORTA = 0b11111111;
	PORTD = 0b00010000;
	
	sbi(PORTC,PC5);
	sbi(PORTC,PC4);
	sbi(PORTC,PC3);
	
	TIMSK= (1<<TOIE1);
	
	sei();
	

	while(1)
	{
		t=refdata[3];
		h=refdata[4];
		p=refdata[5];
		itoa(t,Temp,10);
		itoa(h,Hum,10);
		itoa(p,Pow,10);
		if (t<=9)
		{
			Temp[1]='0';
			}else{
			tm =Temp[1];
			Temp[1]=Temp[0];
			Temp[0]=tm;
		}
		if (h<=9)
		{
			Hum[1]='0';
			}else{
			tm=Hum[1];
			Hum[1]=Hum[0];
			Hum[0]=tm;
		}
		if (p<=9)
		{
			Pow[1]='0';
			}else{
			tm=Pow[1];
			Pow[1]=Pow[0];
			Pow[0]=tm;
		}
		if ( refdata[0] == 0 )
		{
			cbi(LED_PORT,LED_RED);
			sbi(LED_PORT,LED_ORANGE);
			sbi(LED_PORT,LED_GREEN);
		}else if (refdata[6] == 1)
		{
			sbi(LED_PORT,LED_RED);
			sbi(LED_PORT,LED_ORANGE);
			cbi(LED_PORT,LED_GREEN);
		}else
		{
			sbi(LED_PORT,LED_RED);
			cbi(LED_PORT,LED_ORANGE);
			sbi(LED_PORT,LED_GREEN);
		}
		
		Tmptr[0] = display(Temp[0]);
		Tmptr[1] = display(Temp[1]);
		
		Hmdty[0] = display(Hum[0]);
		Hmdty[1] = display(Hum[1]);
		
		Pwr[0] = display(Pow[0]);
		Pwr[1] = display(Pow[1]);
		
		// Temp LED
		cbi(LED_PORT,LED_TEMP);
		sbi(LED_PORT,LED_HUM);
		sbi(LED_PORT,LED_WATT);
		sbi(PORTC,PC4);	

				
			shift_byte(Tmptr[0], Tmptr[1], 0x60);
			_delay_ms(delay1);
		
		
		sbi(LED_PORT,LED_TEMP);
		cbi(LED_PORT,LED_HUM);
		sbi(LED_PORT,LED_WATT);		
		sbi(PORTC,PC4);
			
			shift_byte(Hmdty[0], Hmdty[1], 0xDA);
			_delay_ms(delay1);
		

		sbi(LED_PORT,LED_TEMP);
		sbi(LED_PORT,LED_HUM);
		cbi(LED_PORT,LED_WATT);
		cbi(PORTC,PC4);
			
			shift_byte(Pwr[0], Pwr[1], 0xF2);
			_delay_ms(delay1);
		
		
		cli();	
		_delay_ms(1);
		
		if (eeprom_wr == 0xAB)
		{
			eeprom_update_byte((uint8_t*)16,refdata1[0]);
			eeprom_update_byte((uint8_t*)17,refdata1[1]);
			eeprom_update_byte((uint8_t*)18,refdata1[2]);
			eeprom_wr= 0xAC;
		}
		sei();

	}
	return(0);
}

int display(char ch)
{
	char val = 0;
	switch(ch)
	{
		case '0':
		val = 0xFC;
		break;
		case '1':
		val = 0x60;
		break;
		case '2':
		val = 0xDA;
		break;
		case '3':
		val = 0xF2;
		break;
		case '4':
		val = 0x66;
		break;
		case '5':
		val = 0xB6;
		break;
		case '6':
		val = 0xBE;
		break;
		case '7':
		val = 0xE0;
		break;
		case '8':
		val = 0xFE;
		break;
		case '9':
		val = 0xF6;
		break;
	}
	
	return val;
}

ISR(USART0_RXC_vect)
{
	data= UDR0;
	header[0]=header[1];
	header[1]=header[2];
	header[2]=data;
	//UART_1_putc(0xAB);
	if (takedata==1)
	{
		refdata[refcounter]=data;
		refcounter++;
		//UART_1_putc(0xAC);
	}
	if ((header[0]==0xFF)&&(header[1]==0xFE)&&(header[2]==0xFD))
	{
		refcounter=0;
		takedata=1;
		//UART_1_putc(0xAD);
	}
	if (refcounter==7)
	{
		takedata=0;
		refcounter=0;
		UART_1_putc(0xAA);
		TCNT1 = 56165;
		//TCNT1 = 18685;
		
		TCCR1B |= (1<<CS12);
		//Enable Interrupt
		//TIMSK= (1<<TOIE1);
	}
}

ISR(USART1_RXC_vect)
{
	data1= UDR1;
	
	header1[0]=header1[1];
	header1[1]=data1;
		
	if (takedata1==1)
	{
		refdata1[refcounter1]=data1;
		refcounter1++;
	}
	if ((header1[0]==0x28)&&(header1[1]==0x2B))
	{
		refcounter1=0;
		takedata1=1;
	}
	
	if (refcounter1>=3)
	{
		eeprom_wr = 0xAB;
		crp  =  refdata1[0];
		crp2 =  crp;
		mode =  refdata1[1];
		power=  refdata1[2];
		refcounter1=0;
		takedata1=0;
	}
}

ISR(TIMER1_OVF_vect)
{
	sbi(PORTD,DE);
	sbi(PORTD,RE);
	
	
	UART_0_putc(0xFA);
	UART_0_putc(crp2);
	UART_0_putc(mode);
	UART_0_putc(power);
	UART_0_putc(0xFA);
	
	/* // For Debugging
	UART_0_putc(0xFA);
	UART_0_putc(takedata1);
	UART_0_putc(refcounter1);
	UART_0_putc(eeprom_wr);
	UART_0_putc(0xFA);
	*/ 
	
	_delay_ms(6);
	
	cbi(PORTD,DE);
	cbi(PORTD,RE);
	
	TCCR1B &= ~(1<<CS12);
	//sbi(TIFR,TOV1);
	//TIMSK= 0x00;
}
