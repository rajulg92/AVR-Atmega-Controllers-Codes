/*
 * YASKAWA_MPPT_mCS.c
 *
 * Created: 08-01-2016 15:48:32
 *  Author: RajulGupta
 */ 

#define  F_CPU 8000000UL

#define read_Response  7
#define write_Response 8

#define read_Command_Send_Count  6
#define write_Command_Send_Count 11

#define RE PB1
#define DE PB0


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>

#include "UART_0.h"
#include "UART_1.h"
#include "lcd_lib.h"

uint8_t crc_correct=0,response[8],rx_counter=0;
double Frequency_Update=1.0;

double MPPT(double vdc, double idc, uint16_t FRQ, uint16_t FRQ_previous, double* del_i,double* del_v, double* del_i_v, uint8_t uv);
uint16_t CRC16_2(uint8_t *buf, uint8_t len);
void Send_Command(uint8_t *cmd, uint16_t *value, uint8_t read_count, uint8_t write_count);
void Init_ADC();
double Input_Current();


int main(void)
{
	// To stabilize the Oscillator
	
	_delay_ms(100);
	DDRD  = 0x0A;
	DDRB  = 0x03;
	PORTD = 0xFF ;
	DIDR0= 0xFF;	

	// InItilize UART Baud=9600,8 bit, 1 Stop bit, No Parity
	UART_0_Init();
	UART_1_Init();
	LCDinit();
	LCDcursorOFF();
	Init_ADC();
	
	
	LCDclr; // Clear LCD
	LCDhome();
	LCDGotoXY(0,0);
	LCDstring("  ecoZen        ",16);
	LCDGotoXY(0,1);
	LCDstring("    Solutions   ",16);
	
	//Define variables
	uint8_t Dry_Run=0,toggle=0, DryRunCount=0, NoFault=1, MotorSwitch=0, uv=0;
	uint16_t DCV=0,OPW=0,OVO=0,OCU=0,FL1=0,DIP=0,FRQ=0,FRQ_previous=0,Dummy=0,FRQ_Write=0; 
	char ch1[3],ch2[5],ch3[6],ch4[6],ch5[6],ch6[16],ch7[16],ch8[8], ch9[6],ch10[6],ch11[6],ch12[6],ch13[6];
	double Frequency=0.0, I_current=0.0, FRQ_Float=0.0, OPW_Float=0.0, OCU_Float=0.0;
	
	double New_FRQ=0.0,delta_i=0.0,delta_v=0.0,delta_i_v=0.0;
		
	//Read Commands
	uint8_t DCV_Read_CMD[]={0x02,0x03,0x00,0x31,0x00,0x01};		// DC Bus Voltage
	uint8_t FRQ_Read_CMD[]={0x02,0x03,0x00,0x24,0x00,0x01};		// Frequency
	uint8_t OPW_Read_CMD[]={0x02,0x03,0x00,0x27,0x00,0x01};		// Output Power
	uint8_t OCU_Read_CMD[]={0x02,0x03,0x00,0x26,0x00,0x01};		// Output Current
	uint8_t OVO_Read_CMD[]={0x02,0x03,0x00,0x25,0x00,0x01};		// Output Voltage
	uint8_t FL1_Read_CMD[]={0x02,0x03,0x00,0x21,0x00,0x01};		// Fault Register 1
	uint8_t DIP_Read_CMD[]={0x02,0x03,0x00,0x2B,0x00,0x01};		// Switch Read
	uint8_t DSR_Read_CMD[]={0x02,0x03,0x00,0x2C,0x00,0x01};		// Drive Status Read; for Baseblock
	
	// write Commands
	uint8_t FRQ_SRT_Write_CMD[]={0x02,0x10,0x00,0x01,0x00,0x02,0x04,0x00,0x01,0x03,0xE8};	// Frequency write in REF 1
	uint8_t FRQ_STP_Write_CMD[]={0x02,0x10,0x00,0x01,0x00,0x02,0x04,0x00,0x08,0x00,0x00};	// Stop Motor
		
	uint8_t firsttime=1; //vg Diagnostic
	
	sei();

	cbi(PORTB,RE);
	sbi(PORTB,DE);
	
	// initial drive reset
	Send_Command(&FRQ_STP_Write_CMD,&Dummy,write_Response,write_Command_Send_Count);
	_delay_ms(10000);
	
	while(1)
    {
		if (firsttime==1 && Frequency==0){
			Frequency = 30.0;
		}
		
		//_delay_ms(1000); //vg commenting out master delay
		//Read Output Frequency
		Send_Command(&FRQ_Read_CMD,&FRQ,read_Response,read_Command_Send_Count);
		//Read Output Voltage
		Send_Command(&OVO_Read_CMD,&OVO,read_Response,read_Command_Send_Count);
		//Read Output Power
		Send_Command(&OPW_Read_CMD,&OPW,read_Response,read_Command_Send_Count);
		//Read Output Current
		Send_Command(&OCU_Read_CMD,&OCU,read_Response,read_Command_Send_Count);
		//Read Fault 1
		Send_Command(&FL1_Read_CMD,&FL1,read_Response,read_Command_Send_Count);
		//Read DC bus Voltage
		Send_Command(&DCV_Read_CMD,&DCV,read_Response,read_Command_Send_Count);
		//Input Current
		I_current=Input_Current();
		
		if (FRQ>3000){
			FRQ_previous = FRQ;
		}
		//Int to float 
		FRQ_Float = (double)FRQ/100.0;
		OPW_Float = (double)OPW/100.0;
		OCU_Float = (double)OCU/10.0;
		
		// Convert Values to ASCII
		itoa(DCV,ch1,10);
		dtostrf(FRQ_Float,3,1,ch2);
		dtostrf(OPW_Float,3,2,ch4);				
		dtostrf(OCU_Float,3,1,ch5);
		//itoa(FRQ,ch2,10);
		//itoa(OPW,ch4,10);
		//itoa(OCU,ch5,10);
		itoa(OVO,ch3,10);
		itoa(FL1,ch6,2);
		dtostrf(I_current,3,2,ch8);
		
		dtostrf(delta_i,6,3,ch9);
		dtostrf(delta_v,8,3 ,ch10);
		dtostrf(delta_i_v,6,3,ch11);
		dtostrf(Frequency*10,3,0,ch12);
		
		// LCD Display
		if (NoFault && MotorSwitch){
			if (toggle<4)
			{	
				LCDinit();LCDclr();LCDhome();_delay_ms(1); 
				LCDGotoXY(0,0);	LCDstring("FREQUENCY ",10);	 LCDGotoXY(12,0); LCDstring(ch2,4); 
				LCDGotoXY(0,1);	LCDstring("DC VOLTAGE ",11); LCDGotoXY(13,1); LCDstring(ch1,3); 
				toggle=(toggle+1)%8;
			} 
			else
			{
				LCDinit();LCDclr();LCDhome(); _delay_ms(1);
				LCDGotoXY(0,0);		LCDstring("O/P POWER ",10);		LCDGotoXY(12,0); LCDstring(ch4,4);
				LCDGotoXY(0,1);		LCDstring("O/P CURRENT ",12);	LCDGotoXY(13,1); LCDstring(ch5,3);
				toggle=(toggle+1)%8;
			}
		}
		
		 
		/* Datalog */
		// Input Current
		UART_1_Putc('$');
		UART_1_Puts("A01=");
		UART_1_Puts(ch8);
		// DC Voltage
		UART_1_Puts("&A02=");
		UART_1_Putc(ch1[0]);
		UART_1_Putc(ch1[1]);
		UART_1_Putc(ch1[2]);
		// Output Power
		UART_1_Puts("&A03=");
		UART_1_Puts(ch4);
		// Actual Frequency
		UART_1_Puts("&A04=");
		UART_1_Puts(ch2);
		//vg Output Current
		UART_1_Puts("&A19=");
		UART_1_Puts(ch5);
		
		// Over Current/Motor Short Circuit
		UART_1_Puts("&A05=");
		if ((FL1 & 0x0001)==0x0001)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit();LCDclr();LCDhome(); _delay_ms(1);
			LCDGotoXY(0,0);		LCDstring("  OC/SHORT CKT  ",16);
			LCDGotoXY(0,1);		LCDstring(" PLEASE RESTART ",16);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// DC Over Voltage
		UART_1_Puts("&A06=");
		if ((FL1 & 0x0002)==0x0002)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit();LCDclr();LCDhome(); _delay_ms(1);
			LCDGotoXY(0,0);		LCDstring("  OVER VOLTAGE  ",16);
			LCDGotoXY(0,1);		LCDstring(" PLEASE RESTART ",16);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// Motor OverLoad
		UART_1_Puts("&A07="); 
		if ((FL1 & 0x0004)==0x0004)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit();LCDclr();LCDhome(); _delay_ms(1);
			LCDGotoXY(0,0);		LCDstring(" MOTOR OVERLOAD ",16);
			LCDGotoXY(0,1);		LCDstring(" PLEASE RESTART ",16);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// DC bus under-voltage
		UART_1_Puts("&A12=");
		if ((FL1 & 0x0800)==0x0800 || (FL1 & 0x1000)==0x1000)
		{ 
			UART_1_Putc('1');
			// to reset the drive on UV
			Send_Command(&FRQ_STP_Write_CMD,&Dummy,write_Response,write_Command_Send_Count);
			firsttime=0;
			uv=1;
			_delay_ms(5000);
		}
		else
		{
			UART_1_Putc('0');
			uv=0;
		}
		
		// Motor Phase Loss
		UART_1_Puts("&A13=");
		if ((FL1 & 0x2000)==0x2000)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit();LCDclr();LCDhome(); _delay_ms(1);
			LCDGotoXY(0,0);		LCDstring(" MTR PHASE LOSS ",16);
			LCDGotoXY(0,1);		LCDstring(" PLEASE RESTART ",16);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// Motor Dry Run
		UART_1_Puts("&A15=");
		if (Dry_Run==1)
		{
			UART_1_Putc('1');
		}
		else
		{
			UART_1_Putc('0');
		}
		
		//vg Motor Switch
		UART_1_Puts("&A16=");
		if (MotorSwitch==1)
		{
			UART_1_Putc('1');
		}
		else
		{
			UART_1_Putc('0');
		}
		
		UART_1_Putc('#'); //vg data log terminator
		

		
		// Diagnostic
		// Display Values on UART 1
		/*UART_1_Puts("DCV=");	UART_1_Puts(ch1);
		UART_1_Puts(" FRC=");	UART_1_Puts(ch12);
		UART_1_Puts(" FRQ=");	UART_1_Puts(ch2);
		/*UART_1_Puts(" OVO=");	UART_1_Puts(ch3);
		UART_1_Puts(" OPW=");	UART_1_Puts(ch4);
		UART_1_Puts(" OCU=");	UART_1_Puts(ch5);
		UART_1_Puts(" FL1=");	UART_1_Puts(ch6);
		UART_1_Puts(" FL2=");	UART_1_Puts(ch7); 
		UART_1_Puts(" DCI=");	UART_1_Puts(ch8); 
		UART_1_Puts(" dI=");	UART_1_Puts(ch9); 
		UART_1_Puts(" dV=");	UART_1_Puts(ch10); UART_1_Puts("\r\n");
		//UART_1_Puts(" dI_dV=");	UART_1_Puts(ch11); */
		
		// vg Diagnostic
		if (firsttime==1){
			if (Frequency<87.0){
				Frequency = NoFault*(Frequency + 1.0);
				_delay_ms(200);
			}
		}else 
		{
			Frequency = NoFault*MPPT((double)DCV, I_current, FRQ, FRQ_previous, &delta_i, &delta_v, &delta_i_v, uv);	
		}
		
		//Motor Switch Sensing
		Send_Command(&DIP_Read_CMD,&DIP,read_Response,read_Command_Send_Count);
		if ((DIP & 0x0004)!=0x0004){
			Frequency = 0.0;
			MotorSwitch = 0;
			firsttime = 1;
			LCDinit();LCDclr();LCDhome();_delay_ms(1);
			LCDGotoXY(0,0);		LCDstring(" PLEASE  SWITCH ",16);
			LCDGotoXY(0,1);		LCDstring("MOTOR/SUPPLY  ON",16);
		} else {
			MotorSwitch = 1;
		}
		
		FRQ_Write = ceil(Frequency*100);
		FRQ_SRT_Write_CMD[9] = FRQ_Write>>8;
		FRQ_SRT_Write_CMD[10]= FRQ_Write;
		Send_Command(&FRQ_SRT_Write_CMD,&Dummy, write_Response, write_Command_Send_Count);
		
		
		
    }
		
	return 0;
}

void Init_timer_1()
{
	//TCNT1 = 56165;
	//TCCR1B |= (1<<CS12)|(1<<CS10);
}

void Init_ADC()
{
	//Pre-scalar /8, Interrupt Disable, Auto Trigger off
	ADCSRA = 0b10000111;
	//ADC-0
	ADMUX  = 0b01000000;
}

double Input_Current()
{
	uint8_t i=0,j=0;
	uint16_t filter_array[100],t;
	
	for (i=0;i<100;i++)
	{
		_delay_us(50);
		ADCSRA |= (1<<ADSC);
		while(!(ADCSRA & (1<<ADIF)));
		ADCSRA|=(1<<ADIF);
		filter_array[i] = ADC;
	}
	
	for(i=1;i<100;i++)
	{
		for(j=0;j<100-i;j++)
		{
			if(filter_array[j]>=filter_array[j+1])
			{
				t=filter_array[j];
				filter_array[j]=filter_array[j+1];
				filter_array[j+1]=t;
			}
		}
	}
	
	int sum=0;
	for (uint8_t count=45;count<54;count++)
	{
		sum=sum+filter_array[count];
	}
	
	double result = (((double)sum/10)-459.0)/18.52;
	//double result = ((double)sum/10);
	
	return result;
} 

double MPPT(double vdc, double idc, uint16_t FRQ, uint16_t FRQ_previous, double* del_i,double* del_v, double* del_i_v, uint8_t uv)
{
	static double dv=0.0, di=0.0;
	
	static double New_Frq=0.0;
	static double vdc0=0.0, idc0=0.0;
	
	static uint8_t perIncrement=0, immediateDecrement=1;
	
	if (Frequency_Update==0){
		dv=vdc-vdc0;
	}else {
		dv=vdc-vdc0 + dv;
	}
	
	di=idc-idc0;
	
	Frequency_Update++;
	
	if (FRQ>FRQ_previous && dv>0){
		//vg noise - if I increase freq, voltage should decrease; so do nothing
	}else {
		if (dv/vdc<-0.1 && immediateDecrement==1){
			New_Frq = 0.009*(double)FRQ;
			perIncrement=0;
			Frequency_Update=0;
			immediateDecrement=0;
		}
		if (dv/vdc<-0.02 && (Frequency_Update>2 || perIncrement>=1)){
			New_Frq = 0.0095*(double)FRQ;
			perIncrement=0;
			Frequency_Update=0;
			immediateDecrement=1;
		} 
		if (dv/vdc<-0.005 && (Frequency_Update>2 || perIncrement>=1)){
			New_Frq = New_Frq-0.3;
			perIncrement--;
			Frequency_Update=0;
			immediateDecrement=1; 
		} 
		if (Frequency_Update>2){
			New_Frq=New_Frq+0.1;
			Frequency_Update=0;
			perIncrement=3;
			immediateDecrement=1;
		}
	}
	
	if (uv==1){
		New_Frq=0.0095*(double)FRQ_previous;
		perIncrement=0;
		Frequency_Update=0;
		immediateDecrement=0;
	}
	
	if(New_Frq<30)
	{
		New_Frq=30.0;
	}
	
	if(New_Frq>87)
	{
		New_Frq=87.0;
	}
	
	// storing previous value of v and i
	vdc0 = vdc;
	idc0 = idc;
		
	*del_i	= di;
	*del_v	= dv;
	*del_i_v= dv/vdc;
	
	return New_Frq;
}

void Send_Command(uint8_t* cmd, uint16_t* value, uint8_t read_count,uint8_t write_count)
{
	uint8_t rx_try=9;
	uint16_t crc_sent=0, crc_received=0, crc_calculated=0;
	crc_sent = CRC16_2(cmd, write_count);
 	while (rx_try>0) 
 	{
		sbi(PORTB,RE);	 
		for (uint8_t write_byte=0; write_byte < write_count; write_byte++)
		{
			UART_0_Putc(cmd[write_byte]);	
		}
			
		UART_0_Putc(crc_sent);
		UART_0_Putc(crc_sent>>8);
				
		_delay_ms(4);
		cbi(PORTB,RE);
		
		//Response Read
		for( uint8_t read_byte=0; read_byte < read_count; read_byte++)
		{
			while ( !(UCSR0A & (1<<RXC0)) )
			;
			response[read_byte] = UDR0;
		}

		_delay_ms(8);
		
		crc_received = (response[read_count-1]<<8) | response[read_count-2];
		crc_calculated = CRC16_2(&response,read_count-2);	
		
		if ( crc_calculated == crc_received)
		{
			break;
		}
		
		rx_try--;
		
	} 
	
	if (read_count == read_Response)
	{
		*value = response[4] + response[3]*256;		
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

/*ISR(TIMER1_OVF_vect)
{
	Frequency_Update = 1.0;
	
	TCCR1B &= ~(1<<CS12|1<<CS10);
}*/