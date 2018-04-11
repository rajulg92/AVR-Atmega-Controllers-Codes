/*
 * MPPT_ABB_CustomChar.c
 *
 * Created: 17-02-2016 11:46:01
 *  Author: RajulGupta
 */ 
/*************************Code Summary*********************************/
/*
1> This code is only used in Ecotron.
2> This code is used to communicate/Control with the ABB ACS355 Drive to fetch the DC Bus Voltage, o/p Power, Frequency,
faults, o/p current, etc.
3> This code also control the LCD present as HMI in Ecotron maintaining the Synchronization between the MPPT IC and DataLogging IC.
4> This code additionally switches Buck On upon pumping the required amount of Water.
5> The Buck should be connected at PA4 pin of MPPT IC. 
4> The Fuse Bits are E2 D9.
*/

#define F_CPU 8000000UL
#define read  7
#define write 8
#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include "UART_0.h"
#include "UART_1.h"
#include "lcd_lib.h"

uint8_t crc_correct=0,assign_response=0,response_recieved=0,response[8],rx_counter=0,skip=0, flag_sms=1,i_isr=0;
double Frequency_Update=1.0;
char com_data[35],data;

/* Function Definition */
double MPPT(double vdc, double idc, uint16_t FRQ, uint16_t FRQ_previous, double* del_i,double* del_v, double* del_i_v, uint8_t uv);
uint16_t CRC16_2(uint8_t *buf, uint8_t len);
void Send_Command(uint8_t *cmd, uint16_t *value, uint8_t read_count);
void Init_ADC();
double Input_Current();

int main(void)
{
	char SmS_ShutOff,signal_strength,signal_status,user[30];
	uint8_t i=0;
	char initial[]={'0','0','0'};
	
	// To stabilize the Oscillator
	_delay_ms(100);
	
	// Port Declaration
	DDRB = 0x00;
	DDRD  = 0x0A;
	PORTD = 0xFF ;
	DIDR0= 0xFF;
	DDRA = 0xF0;

	cbi(PORTA,4);
	// InItilize UART Baud=9600,8 bit, 1 Stop bit, No Parity
	UART_0_Init();
	UART_1_Init();
	LCDinit();
	LCDcustom();
	Init_ADC();
	
	LCDclr; _delay_ms(10);
	LCDhome(); _delay_ms(10);
	LCDGotoXY(0,0); _delay_ms(10);
	LCDstring("  ecoZen        ",16); _delay_ms(10);
	LCDGotoXY(0,1); _delay_ms(10);
	LCDstring("    Solutions   ",16); _delay_ms(10);
	
	// Enable Timer Interrupt
	_delay_ms(1);
	sbi(TIMSK1,TOIE1);
	_delay_ms(1);
	
	//  Waiting for getting initial data (SMS off)
	i = 0;
	while(!(initial[0]==0x0D && initial[1]==0x0A && initial[2]==0x0A)){
		initial[0] = initial[1];
		initial[1] = initial[2];
		while (!(UCSR1A & (1<<RXC1)));
		initial[2] = UDR1;
	}
	while (!(UCSR1A & (1<<RXC1)));
	com_data[0] = UDR1;
	
	UART_1_Putc('*');
	
	//  Enabling interrupts
	sei();
	
	// Initializing data
	SmS_ShutOff=com_data[0];
	signal_status='1';
	signal_strength='0';
	com_data[1]='1';
	com_data[2]='0';
	
	
	_delay_ms(5000);
	
	//Define variables
	uint8_t Dry_Run=0,toggle=0, DryRunCount=0, NoFault=1, MotorSwitch=0, MotorSwitchCount=0, uv=0, firsttime=1,flag_show=0,str_n=0;
	uint16_t DCV=0,OPW=0,OVO=0,OCU=0,FL1=0,FL2=0,FRQ=0,FRQ_previous=0,Dummy=0,FRQ_Write=0;
	char ch1[6],ch2[6],ch3[6],ch4[6],ch5[6],ch6[16],ch7[16],ch8[8], ch9[6],ch10[6],ch11[6],ch12[6],ch13[6];
	double Frequency=0.0, I_current=0.0, VOC=0.0, FRQ_Float=0.0, OPW_Float=0.0, OCU_Float=0.0, New_FRQ=0.0, delta_i=0.0, delta_v=0.0, delta_i_v=0.0;
	
	//Read Commands
	uint8_t DCV_Read_CMD[]={0x01,0x03,0x00,0x6A,0x00,0x01};		// DC Bus Voltage
	uint8_t FRQ_Read_CMD[]={0x01,0x03,0x00,0x66,0x00,0x01};		// Frequency
	uint8_t OPW_Read_CMD[]={0x01,0x03,0x00,0x69,0x00,0x01};		// Output Power
	uint8_t OCU_Read_CMD[]={0x01,0x03,0x00,0x67,0x00,0x01};		// Output Current
	uint8_t OVO_Read_CMD[]={0x01,0x03,0x00,0x6C,0x00,0x01};		// Output Voltage
	uint8_t FL1_Read_CMD[]={0x01,0x03,0x01,0x30,0x00,0x01};		// Fault Register 1
	uint8_t FL2_Read_CMD[]={0x01,0x03,0x01,0x31,0x00,0x01};		// Fault Register 2
	uint8_t DIP_Read_CMD[]={0x01,0x03,0x00,0x03,0x00,0x01};		// Digital Input Terminal
	
	// write Commands
	uint8_t FRQ_Write_CMD[]={0x01,0x06,0x00,0x01,0x1F,0x40};	// Frequency write in REF 1
	uint8_t SRT_Write_CMD[]={0x01,0x06,0x00,0x00,0xFF,0xFF};	// Start Motor
	uint8_t STP_Write_CMD[]={0x01,0x06,0x00,0x00,0xFF,0xF0};	// Stop Motor
	
	Frequency=10.0;
	
	
	Send_Command(&SRT_Write_CMD,&Dummy,write);
	

	while(1)
	{
		
		_delay_ms(300);
		//Read Output Frequency
		Send_Command(&FRQ_Read_CMD,&FRQ,read);
		//Read Output Voltage
		Send_Command(&OVO_Read_CMD,&OVO,read);
		//Read Output Power
		Send_Command(&OPW_Read_CMD,&OPW,read);
		//Read Output Current
		Send_Command(&OCU_Read_CMD,&OCU,read);
		//Read Fault 1
		Send_Command(&FL1_Read_CMD,&FL1,read);
		//Read Fault 2
		Send_Command(&FL2_Read_CMD,&FL2,read);
		//Read DC bus Voltage
		Send_Command(&DCV_Read_CMD,&DCV,read);
		//Input Current
		I_current=Input_Current();
		
		//Int to float
		FRQ_Float = (double)FRQ/10.0;
		OPW_Float = (double)OPW/10.0;
		OCU_Float = (double)OCU/10.0;
		
		// Convert Values to ASCII
		itoa(DCV,ch1,10);
		dtostrf(FRQ_Float,3,1,ch2);
		dtostrf(OPW_Float,3,2,ch4);
		dtostrf(OCU_Float,3,1,ch5);
		itoa(OVO,ch3,10);
		itoa(FL1,ch6,2);
		itoa(FL2,ch7,2);
		dtostrf(I_current,3,2,ch8);
		
		dtostrf(delta_i,6,3,ch9);
		dtostrf(delta_v,6,0,ch10);
		dtostrf(delta_i_v,6,3,ch11);
		dtostrf(Frequency*10,3,0,ch12);
		
		// LCD Display
		if (NoFault){   
			if (toggle==0)
			{
				LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
				LCDGotoXY(0,0); _delay_ms(10);	LCDstring("FREQUENCY   ",12);  _delay_ms(10);	LCDGotoXY(12,0); _delay_ms(10); LCDstring(ch2,4); _delay_ms(10);
				LCDGotoXY(0,1); _delay_ms(10);	LCDstring("DC VOLTAGE   ",13); _delay_ms(10);	LCDGotoXY(13,1); _delay_ms(10); LCDstring(ch1,3); _delay_ms(10);
				toggle=(toggle+1)%5;
			}
			else if (toggle<2 && toggle>0)
			{
				LCDGotoXY(12,0); _delay_ms(10); LCDstring(ch2,4); _delay_ms(10);
				LCDGotoXY(13,1); _delay_ms(10); LCDstring(ch1,3); _delay_ms(10);
				toggle=(toggle+1)%5;
			}
			else if (toggle==2)
			{
				LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
				LCDGotoXY(0,0); _delay_ms(10);	LCDstring("O/P POWER   ",12); _delay_ms(10);	LCDGotoXY(12,0); _delay_ms(10); LCDstring(ch4,4); _delay_ms(10);
				LCDGotoXY(0,1); _delay_ms(10);	LCDstring("O/P CURRENT  ",13); _delay_ms(10);	LCDGotoXY(13,1); _delay_ms(10); LCDstring(ch5,3); _delay_ms(10);
				toggle=(toggle+1)%5;
			}
			else if (toggle<4 && toggle>2){
				LCDGotoXY(12,0); _delay_ms(10); LCDstring(ch4,4); _delay_ms(10);
				LCDGotoXY(13,1); _delay_ms(10); LCDstring(ch5,3); _delay_ms(10);
				toggle=(toggle+1)%5;
			}
			else if (toggle>=4 && signal_status=='1'){
				LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
				LCDGotoXY(0,0); _delay_ms(10);	LCDstring(" Searching for  ",16); _delay_ms(10);
				LCDGotoXY(0,1); _delay_ms(10);	LCDstring("     Network... ",16); _delay_ms(10);
				toggle=(toggle+1)%5;
			} else if (toggle>=4){
				switch (signal_strength){
					case '0':
					LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
					LCDGotoXY(0,0); _delay_ms(10);	LCDsendChar(0);_delay_ms(10);
					LCDGotoXY(4,0); _delay_ms(10);	LCDstring(user,str_n); _delay_ms(10);
					break;
					
					case '1':
					LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
					LCDGotoXY(0,0); _delay_ms(10);	LCDsendChar(0);_delay_ms(10);LCDsendChar(1);_delay_ms(10);
					LCDGotoXY(4,0); _delay_ms(10);	LCDstring(user,str_n); _delay_ms(10);
					break;
					
					case '2':
					LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
					LCDGotoXY(0,0); _delay_ms(10);	LCDsendChar(0);_delay_ms(10);LCDsendChar(2);_delay_ms(10);
					LCDGotoXY(4,0); _delay_ms(10);	LCDstring(user,str_n); _delay_ms(10);
					break;
					
					case '3':
					LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
					LCDGotoXY(0,0); _delay_ms(10);	LCDsendChar(0);_delay_ms(10);LCDsendChar(3);_delay_ms(10);
					LCDGotoXY(4,0); _delay_ms(10);	LCDstring(user,str_n); _delay_ms(10);
					break;
				}
				switch(signal_status){
					case '2':
					LCDGotoXY(0,1); _delay_ms(10);	LCDstring("Searching GPRS..",16); _delay_ms(10);
					break;
					
					case '3':
					LCDGotoXY(0,1); _delay_ms(10);	LCDstring("    GPRS OK     ",16); _delay_ms(10);
					break;
				}
				toggle=(toggle+1)%5;
			}
		}
		
		
		/* Data log */
		//   Initializing ISR variables
		com_data[3]='$';
		com_data[4]='#';
		
		// Input Current
		UART_1_Putc('$');
		UART_1_Puts("A01=");
		UART_1_Puts(ch8);
		// DC Voltage
		UART_1_Puts("&A02=");
		UART_1_Puts(ch1);
		// Output Power
		UART_1_Puts("&A03=");
		UART_1_Puts(ch4);
		// Actual Frequency
		UART_1_Puts("&A04=");
		UART_1_Puts(ch2);
		//vg Output Current
		UART_1_Puts("&A19=");
		UART_1_Puts(ch5);
		
		// Over Current
		UART_1_Puts("&A05=");
		if ((FL1 & 0x0001)==0x0001)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("   OVERCURRENT  ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
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
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("   OVERVOLTAGE  ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// Short Circuit
		UART_1_Puts("&A07=");
		if ((FL1 & 0x0008)==0x0008)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("  SHORT CIRCUIT ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// DC bus under voltage
		UART_1_Puts("&A12=");
		if ((FL1 & 0x0020)==0x0020)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("  UNDERVOLTAGE  ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// I/P Phase Loss
		UART_1_Puts("&A10=");
		if ((FL2 & 0x0020)==0x0020)
		{
			UART_1_Putc('1');
		}
		else
		{
			UART_1_Putc('0');
		}
		
		// Motor Phase Loss
		UART_1_Puts("&A13=");
		if ((FL2 & 0x4000)==0x4000)
		{
			UART_1_Putc('1');
			NoFault = 0;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring(" MTR PHASE LOSS ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
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
		
   /*     //vg Motor Switch
		UART_1_Puts("&A16=");
		if (MotorSwitch==1)
		{
			UART_1_Putc('1');
		}
		else
		{
			UART_1_Putc('0');
		}
	*/	
		//vg Remote On/Off
		UART_1_Puts("&A11=");
		UART_1_Putc(SmS_ShutOff);
		
		UART_1_Putc('#');
		
		//  Reception Enable
		sbi(UCSR1B,RXEN1);
		
		//Waiting for Interrupt Reception
		_delay_ms(1000);
		
		//  Reception disable
		cbi(UCSR1B,RXEN1);
		
		SmS_ShutOff = com_data[0];
		signal_status = com_data[1];
		signal_strength = com_data[2];
		
		//  Parsing network provider
		if (com_data[4]!='#'){
			i = 4;
			while(com_data[i]!='#'){
				user[i-4] = com_data[i];
				i++;
			}
			user[i-4]='\0';
			str_n = i-4;
		}
		
	/*	//Motor Switch sensing
		if (FRQ==0 && NoFault && uv==0){
			MotorSwitchCount++;
			if (MotorSwitchCount>=3){
				MotorSwitchCount = 4;
				MotorSwitch = 0;
				firsttime = 1;
				Frequency = 10.0;
				LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
				LCDGotoXY(0,0); _delay_ms(10);		LCDstring("SWITCH ON MOTOR ",16); _delay_ms(10);
				LCDGotoXY(0,1); _delay_ms(10);		LCDstring("    AND WAIT    ",16); _delay_ms(2000);
			}
			}else {
			MotorSwitch = 1;
			MotorSwitchCount = 0;
		}
		*/
		//vg Dry Run
		if (FRQ>350 && OPW<4){
			DryRunCount++;
			if (DryRunCount>=3){
				DryRunCount = 4;
				Dry_Run = 1;
				NoFault = 0;
				LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
				LCDGotoXY(0,0); _delay_ms(10);		LCDstring("     DRY RUN    ",16); _delay_ms(10);
				LCDGotoXY(0,1); _delay_ms(10);		LCDstring(" PLEASE RESTART ",16); _delay_ms(10);
				Send_Command(&STP_Write_CMD,&Dummy,write);
				
			}
			}else {
			DryRunCount = 0;
		}
		
		if (FRQ>100 && uv==0){
			FRQ_previous = FRQ;
		}
		
		if (SmS_ShutOff==0x30){
			NoFault = 0;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("     SYSTEM     ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring("REMOTELY SHUTOFF",16); _delay_ms(10);
			//  Stop command
			Send_Command(&STP_Write_CMD,&Dummy,write);
			flag_sms=0;
			_delay_ms(5000);
			sbi(PORTA,4);
		}
		else if((SmS_ShutOff==0x31) && (flag_sms==0))
		{
			NoFault=1;
			LCDinit(); _delay_ms(10); LCDclr(); _delay_ms(10); LCDhome(); _delay_ms(10);
			LCDGotoXY(0,0); _delay_ms(10);		LCDstring("   SYSTEM       ",16); _delay_ms(10);
			LCDGotoXY(0,1); _delay_ms(10);		LCDstring("REMOTELY STARTED",16); _delay_ms(10);
			cbi(PORTA,4);
			_delay_ms(5000);
			//  Start command
			Send_Command(&SRT_Write_CMD,&Dummy,write);
			flag_sms=1;
		}
		
	}
	return 0;
}

ISR(USART1_RX_vect)
{
	data = UDR1;
	
	if ((data == 0x0D) && (assign_response == 1))
	{
		response_recieved=1;
		assign_response=0;
		com_data[i_isr]='\0';
	}
	if (assign_response==1)
	{
		com_data[i_isr]=data;
		i_isr++;
	}
	if ((data == 0x0A) && (assign_response == 0))
	{
		assign_response=1;
		i_isr=0;
		
	}
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
	
	double result = (((double)sum/10)-459.0)/18.52;;
	
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
			New_Frq = 0.09*(double)FRQ;
			perIncrement=0;
			Frequency_Update=0;
			immediateDecrement=0;
		}
		if (dv/vdc<-0.02 && (Frequency_Update>2 || perIncrement>=1)){
			New_Frq = 0.095*(double)FRQ;
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
		New_Frq=0.095*(double)FRQ_previous;
		perIncrement=0;
		Frequency_Update=0;
		immediateDecrement=0;
	}
	
	if(New_Frq<10)
	{
		New_Frq=10.0;
	}
	
	if(New_Frq>50)
	{
		New_Frq=50.0;
	}
	
	// storing previous value of v and i
	vdc0 = vdc;
	idc0 = idc;
	
	*del_i	= di;
	*del_v	= dv;
	*del_i_v= dv/vdc;
	
	return New_Frq;
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

		// Start Timer
		skip = 0;
		TCNT1 = 60000;
		sbi(TCCR1B,CS12);
		
		//Response Read
		for( uint8_t read_byte=0; read_byte<read_count; read_byte++)
		{
			while ( !(UCSR0A & (1<<RXC0)) )
			{
				_delay_us(10);
				if (skip == 1)
				{
					break;
				}
			}
			response[read_byte] = UDR0;
		}
		
		if (read_count == read)
		{
			crc_received = (response[6]<<8) | response[5];
			crc_calculated = CRC16_2(&response,5);
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

ISR(TIMER1_OVF_vect)
{
	skip = 1;
	cbi(TCCR1B,CS12);
}