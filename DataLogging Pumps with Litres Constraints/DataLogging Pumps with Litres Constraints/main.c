/*
 * DataLogging.c
 *
 * Created: 27-08-2015 12:13:32
 *  Author: RajulGupta
 */ 

/********************Code Summary***************************************/
/*
1> The following code is used for dataLogging in Solar Pumps only.
2> For Enabling the DataLogging on any serial no. send SMS via Master Mobile no. i.e. +918605008162
in a pattern "<Serial no>" for example if serial no. is EZMCI01160000 the we'll send the SMS as EZMCI01160000
from the Master no to the SIM Card no. installed in that system.
3> To register Customer no. so that the Customer will be able to ON/OFF the system please send +91<customer no> i.e.
if customer no is 8605029237 then send SMS as +918605029237 from the Master mobile no to the SIM card no. installed in that system.
After registering the no. now only Master and Customer will be able to ON/OFF system.
4> To ON/OFF system send SMS ON or OFF(regarless of upper case and lower case) from Master and Customer Mobile no
5> In the Following Code the Two Additional nos are added to recieve the SMS from i.e. VM-40060 and VK-40060.
6> Fuse bits used should be E2 D9
*/
// USART 1 FOR SIM900A
// USART 0 FOR DATA TRANSMISSION 

#define F_CPU 8000000UL		//freq 16MHz

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <math.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <compat/deprecated.h>
#include <avr/eeprom.h>
#include "UART_1.h"
#include "UART_0.h"

#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;
#define MASTER_NUMBER "+918605029237\0"

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define NumberOfAttempts 10

//  Global variables for Timer
uint8_t skip=0,overflowcount=0;

//  Function Definition 
uint8_t get_apn(char* apn,char* user);
void sim900_cmd(char* str,char* response);
uint8_t get_sms(uint8_t* system_on,char* customer_no,char* Serial_num,int* no_litres);
void dataLog(uint8_t system_on,char* apn,char* Serial_num,uint8_t signal_strength,char* user,char* loc_lat,char* loc_long);
uint8_t fetch_gprs();
void get_data(uint8_t system_on,char* responseimp,uint8_t  signal_status,uint8_t signal_strength,char* user);
uint8_t set_apn(char* apn);
uint8_t allocate_dynaIP();
uint8_t Log_data(char* responseimp,char* Serial_num,char* loc_lat,char* loc_long);
void sim900_response(char* response);
uint8_t get_signalstrength();
void start_timer();
uint8_t wait_for_data();
void get_loc(char* apn,char* loc_lat,char* loc_long);



void main(){
	
	uint8_t flag_get_apn=0,flag_get_sms=0,system_on,signal_strength=0;
	uint8_t dummy,data_counter=0;
	int e=0,l=0,i=0,no_litres=0;
	char Serial_num[20],customer_no[20],apn[30],user[30],responseimp[512],loc_lat[15],loc_long[15];
		
	DDRD = 0xFA;
	DDRB = 0xFF;
	
	UART_0_init();
	UART_1_init();
	
	// Enable Timer Interrupt
	_delay_ms(1);
	sbi(TIMSK1,TOIE1);
	_delay_ms(1);
	
	//  Enabling interrupts
	sei();
	
	// To read previous system on/off status
	system_on=eeprom_read_byte((uint8_t*)30);
	
	//  Reception Enable 
	sbi(UCSR0B,RXEN0);
	
	// Start Timer
	start_timer();
	
	//Sending the EEPROM data and waiting for the acknowledgement
	while(skip==0){  //  skip is global timer variable
		UART_0_puts("\r\n\n");
		UART_0_putc(system_on);
		UART_0_puts("\r\n");
		_delay_ms(1000);
		if (UCSR0A & (1<<RXC0)){
			dummy=UDR0;
			if(dummy=='*'){
				//  Reception Disable
				cbi(UCSR0B,RXEN0); 
				break;
			}
		}
	}
	
	//  Stop Timer
	cbi(TCCR1B,CS12);
	
	// To read last updated Serial No. of controller
	e=32;l=0;i=0;
	while (l<13){
		Serial_num[i]=eeprom_read_byte((uint8_t*)e);
		i++;e++;l++;
	}
	Serial_num[i]='\0';
	
	// To read last updated customer number
	e=16;i=0;
	while (i<13){
		customer_no[i]=eeprom_read_byte((uint8_t*)e);
		i++;e++;
	}
	customer_no[i]='\0';
	
	
	// Delay for SIM900 initialization
	_delay_ms(10000);
	
	while(1){
		
		// To fetch APN
		while (flag_get_apn!=1){
			flag_get_apn = get_apn(apn,user);
		}
		
		//  Get Location
		get_loc(apn,loc_lat,loc_long);
		
		//  To get signal strength
		signal_strength=get_signalstrength();
		
		//  To send network info; signal status is 2: GPRS acquiring...
		get_data(system_on,&responseimp,2,signal_strength,user);

		// To fetch SMS 	
		data_counter=NumberOfAttempts; // Number of attempts to be made if SMS fetch fails
		while (data_counter!=0){
			flag_get_sms = get_sms(&system_on,customer_no,Serial_num,&no_litres);
			data_counter--;
			if(flag_get_sms==1){  // SMS Fetch successful
				break;
			}
		}
		
	//	UART_1_puts("\r\n\r\n");  // String terminators to eliminate noise etc.
		
 		// Logging Data
		dataLog(system_on,apn,Serial_num,signal_strength,user,loc_lat,loc_long);	
		
		_delay_ms(60000);  // Delay between 2 data logging samples
	}	
 }
 
uint8_t get_apn(char* apn,char* user){
	 
	uint8_t i=0,p=0;
	char simresponse[200];
	
	// SIM900 Echo disabled in response
	UART_1_puts("ATE0\r\n\0");  
	_delay_ms(500);
	
	// SIM900 module check
	sim900_cmd("AT\r\n\0",simresponse);
	if (strcmp(simresponse,"OK\0")!=0){
		return 0;
	}
	
	// To check if SIM card is present	 
	sim900_cmd("AT+CSMINS?\r\n\0",simresponse); 
	if (strcmp(simresponse,"+CSMINS: 0\,1\0")!=0){
		return 0;
	}
	
	// To check if SIM card is registered
	sim900_cmd("AT+CREG?\r\n\0",simresponse);  
	if (!((strcmp(simresponse,"+CREG: 0\,1\0")==0) || (strcmp(simresponse,"+CREG: 0\,5\0")==0))){
		sim900_cmd("AT+CREG?\r\n\0",simresponse);
		return 0;
	}
	
	// Returns name of Network Provider
	sim900_cmd("AT+CSPN?\r\n\0",simresponse);  
	i=0;p=0;
	while (simresponse[i]!='\"' && i<200){
		i++;
	}
	i++;
	while (simresponse[i]!='\"' && p<30){
		user[p]=simresponse[i];
		p++;i++;
	}
	user[p]='\0';
					 
	//APN Compare and Assignment
	if (strcmp(user,"Hutch\0")==0){
		strcpy(apn,"www");
		return 1;
	}
	else if (strcmp(user,"Vodafone\0")==0){
		strcpy(apn,"www");
		return 1;
	}
	else if ((strcmp(user,"airtel\0")==0) || (strcmp(user,"Airtel\0")==0)){
		strcpy(apn,"airtelgprs.com");
		return 1;
	}
	else if(strcmp(user,"CellOne\0")==0){
		strcpy(apn,"bsnlenet");
		return 1;;
	}
	else if(strcmp(user,"Reliance\0")==0){
		strcpy(apn,"rcomwap");
		return 1;
	}
	else if(strcmp(user,"TATA\0")==0){
		strcpy(apn,"tata.docomo.internet");
		return 1;
	}
	else if(strcmp(user,"Uninor\0")==0){
		strcpy(apn,"uninor");
		return 1;
	}
	else if(strcmp(user,"!dea\0")==0){
		strcpy(apn,"internet");
		return 1; 
	}
	else{
		return 0;
	}
}

void sim900_cmd(char* str,char* response){	
	uint8_t i=0;
	char initial[]={'0','0'};

	//  Send Command
	while( *str != '\0' ){
		UART_1_putc( *str++ );
	}
	
	//  Reception Enable
	sbi(UCSR1B,RXEN1);
	
	sim900_response(response);
}

uint8_t get_sms(uint8_t* system_on,char* customer_no,char* Serial_num,int* no_litres){
	
	uint8_t i=0,p=0,dataarrived=0;
	uint8_t update_customer_no=0,update_serial_no=0,update_remote_control=0;
	int no_of_msg,message_number;
	char no_msg[6],response_number[150],contact_no[20],simresponse[30],msg[160],dummy[5];
	char initial[]={'0','0'};
	char litres[20];
	// Check no. of messages
	sim900_cmd("AT+CPMS=\"SM\"\r\n\0",simresponse);
	
	//  For re-enabling reception for OK
	sbi(UCSR1B,RXEN1);
	
	sim900_response(dummy);
	
	i=0;p=0;
	while (simresponse[i]!='\:' && i<30){
		i++;
	}
	i++;i++;
	while (simresponse[i]!='\,' && p<6){
		no_msg[p]=simresponse[i];
		p++;i++;
	}
	no_msg[p]='\0';
	no_of_msg = atoi(no_msg);  
	
	// Exiting the function if no new messages have arrived
	if (no_of_msg==0){
		return 1;
	}
	
	// Changing the message to readable format
	sim900_cmd("AT+CMGF=1\r\n",simresponse);
	if (strcmp(simresponse,"OK\0")!=0){
		return 0;
	}
	
	// To fetch SMS messages
	message_number=1;
	while (message_number<=no_of_msg){
		initial[0]='0';initial[1]='0';  // Re-initializing initial variable
		
		// Send command for reading the particular message
		itoa(message_number,no_msg,10);
		UART_1_puts("AT+CMGR=");
		UART_1_puts(no_msg);
		UART_1_puts("\r\n");
		
		//  For enabling reception
		sbi(UCSR1B,RXEN1);		
		
		//  For receiving sender number
		while(!(initial[0]==0x0D && initial[1]==0x0A)){
			initial[0]=initial[1];
			dataarrived=wait_for_data();
			if (dataarrived==0){
				break;
			}
			initial[1]=UDR1;
		}

		i=0;
		
		while(1){
			dataarrived=wait_for_data();
			if (dataarrived==0){
				break;
			}
			response_number[i]=UDR1;
			if(response_number[i]==0x0D || i>=150){
				break;
			}
			i++;
		}
		
		//  Terminating strings
		response_number[i]="\0";
		
		//  For those odd unexplainable cases where this response comes empty with OK
		if (strcmp(response_number,"OK\0")==0){
			message_number++;
			continue;
		}
		
		//  Receiving message
		p=0;
		
		dataarrived=wait_for_data();
		if (dataarrived==1){
			initial[1]=UDR1;  // To eliminate Lf that comes after Cr but before message starts;
		}
		
		while(1){
			dataarrived=wait_for_data();
			if (dataarrived==0){
				break;
			}
			msg[p]=UDR1;
			if(msg[p]==0x0D || p>=160){
				break;
			}
			p++;
		}
		
		//  For receiving final OK
		sim900_response(dummy);

		//  Terminating strings
		msg[p]="\0";	
		
		// Figuring out Phone number of SMS sender
		i=0;p=0;
		while (response_number[i]!='\"' && i<150){
			i++;
		}
		i++;
		while (response_number[i]!='\"' && i<150){
			i++;
		}
		i++;
		while (response_number[i]!='\"' && i<150){
			i++;
		}
		i++;
		while (response_number[i]!='\"' && p<20 && i<150){
			contact_no[p]=response_number[i];
			p++;i++;
		}
		contact_no[p]='\0';
		 UART_1_puts(contact_no);
 	     UART_1_puts(msg);
		 UART_1_puts("\r\n");
		 _delay_ms(5000);
		// If message doesn't match authenticated source, move to next message
		if (!((strcmp(contact_no,customer_no)==0) || (strcmp(contact_no,MASTER_NUMBER)==0) || (strcmp(contact_no,"VK-040060")==0) || (strcmp(contact_no,"VM-040060")==0))){
			message_number++;
			continue;
		}
		
		if ((msg[0]=='+') && (strcmp(contact_no,MASTER_NUMBER)==0)){  //  Changing customer number by Master
			strcpy(customer_no,msg);
			update_customer_no=1;
		}
		else if ((strcasecmp(msg,"on")==0) && ((strcmp(contact_no,MASTER_NUMBER)==0) || (strcmp(contact_no,customer_no)==0) || (strcmp(contact_no,"VK-040060")==0) || (strcmp(contact_no,"VM-040060")==0))){  //  Commanding system to Switch on by Customer/Master
			*system_on=0x31;
			update_remote_control=1;
		}
		else if((strcasecmp(msg,"off")==0) && ((strcmp(contact_no,MASTER_NUMBER)==0) || (strcmp(contact_no,customer_no)==0) || (strcmp(contact_no,"VK-040060")==0) || (strcmp(contact_no,"VM-040060")==0))){  //  Commanding system to Switch off by Customer/Master
			*system_on=0x30;
			update_remote_control=1;
		}
		else if ((strncmp(msg,"EZMCI",5)==0) && (strcmp(contact_no,MASTER_NUMBER)==0)){  //  To change serial number by Master
			strcpy(Serial_num,msg);
			update_serial_no=1;
		}
		else if ((strncasecmp(msg,"litres:",7)==0))	{
			strcpy(litres,msg+7);
			no_litres=atoi(litres);
		}
		message_number++;
	}
	
	// To update customer number in EEPROM
	if (update_customer_no){
		i=0;
		while (i<=12){
			eeprom_update_byte((uint8_t*)(i+16),customer_no[i]);
			_delay_ms(5);  //  EEPROM write time is 3.3ms in data sheet
			i++;
		}
	}
	
	// To update serial number in EEPROM
	if (update_serial_no){
		i=0;
		while (i<=12){
			eeprom_update_byte((uint8_t*)(i+32),Serial_num[i]);
			_delay_ms(5);  //  EEPROM write time is 3.3ms in data sheet
			i++;
		}
	}
	
	// To update system on/off status in EEPROM
	if (update_remote_control){
		eeprom_update_byte((uint8_t*)30,*system_on);
		_delay_ms(5);  //  EEPROM write time is 3.3ms in data sheet
	}
		
	// To delete all messages
	sim900_cmd("AT+CMGDA=\"DEL READ\"\r\n",simresponse);
	
	return 1;
}

void dataLog(uint8_t system_on,char* apn,char* Serial_num,uint8_t signal_strength,char* user,char* loc_lat,char* loc_long){
	uint8_t flag_fetch_gprs=0,flag_set_apn=0,flag_allocate_dynaIP=0,flag_Log_data=0;
	uint8_t data_counter=0;
	char responseimp[512];
	
	// To acquire GPRS settings
	data_counter=NumberOfAttempts;  // Number of attempts to be made if APN fetch fails
	while (data_counter!=0){
		flag_fetch_gprs = fetch_gprs();
		data_counter--;
		if(flag_fetch_gprs==1){  // GPRS settings Fetch successful
			break;
		}
		else if(data_counter==0){  // Number of attempts exhausted
			return;
		}	
	}
	
	// Acquiring data for logging and setting APN
	data_counter=NumberOfAttempts;  // Number of attempts to be made if APN set fails
	while (data_counter!=0){
		// Acquiring data for logging		
		get_data(system_on,&responseimp,2,signal_strength,user);
		
		//Setting APN
		flag_set_apn = set_apn(apn);
		data_counter--;
		if(flag_set_apn==1){  // APN set successful
			break;
		}
		else if(data_counter==0){  // Number of attempts exhausted
			return;
		}		
	}
	
	// Allocating dynamic IP
	data_counter=NumberOfAttempts;  // Number of attempts to be made if Dynamic IP allocation fails
	while (data_counter!=0){
		flag_allocate_dynaIP = allocate_dynaIP();
		data_counter--;
		if(flag_allocate_dynaIP==1){  // Dynamic IP allocation successful
			break;
		}
		else if(data_counter==0){  // Number of attempts exhausted
			return;
		}		
	}
	
	// Data Logging
	data_counter=NumberOfAttempts;  // Number of attempts to be made if Dynamic IP allocation fails
	while (data_counter!=0){
		flag_Log_data = Log_data(responseimp,Serial_num,loc_lat,loc_long);
		data_counter--;
		if(flag_Log_data==1){  // Data Logging successful
			break;
		}
		else if(data_counter==0){  // Number of attempts exhausted
			return;
		}		
	}
	
	//  To send network info; signal status is 3: GPRS OK
	get_data(system_on,&responseimp,3,signal_strength,user);
}

uint8_t fetch_gprs(){
	char simresponse[20];
	
	sim900_cmd("AT+CGATT?\r\n",simresponse);
	if(strcmp(simresponse,"+CGATT: 0\0")==0){  // If GPRS is not attached
		_delay_ms(10000);
	}else if(strcmp(simresponse,"+CGATT: 1\0")==0){  // GPRS is attached properly
		//  Reception Enable again for OK
		sbi(UCSR1B,RXEN1);
		
		sim900_response(simresponse);
		return 1;
	}
	return 0;
}

void get_data(uint8_t system_on, char* responseimp,uint8_t  signal_status, uint8_t signal_strength,char* user){
	uint8_t t=0,response_receivedimp=0,takedata=0,dataarrived=0;
	char dataimp,ch1[3],ch2[3];
	
	//  Integer to ASCII for transmission
	itoa(signal_status,ch1,10);
	itoa(signal_strength,ch2,10);
	
	//  Reception Enable
	sbi(UCSR0B,RXEN0);
	
	while(response_receivedimp==0){
		while(!((UCSR0A) & (1<<RXC0)));  // Wait while data is being received
		dataimp = UDR0;
		
		// If last character # is received.. 
		if ((dataimp == 0x23) && (takedata == 1)){  //  #=0x23
			//  Reception Disable
			cbi(UCSR0B,RXEN0);
			
			responseimp[t]='\0';
			response_receivedimp=1;
			takedata = 0;
			
			//  Sending data for displaying network health on LCD and SMS shutoff
			UART_0_puts("\r\n");
			UART_0_putc(system_on);
			UART_0_puts(ch1);
			UART_0_puts(ch2);
			UART_0_putc('$');
			UART_0_puts(user);
			UART_0_putc('#');
			UART_0_puts("\r\n");
		}
		// Continue taking data
		if (takedata==1 && t<512){
			responseimp[t]=dataimp;
			t++;
		}
		// If first character $ is received, start taking data..
		if ((dataimp == 0x24) && (takedata == 0)){  //  $=0x24
			takedata=1;
		}
	}
}

uint8_t set_apn(char* apn){
	char simresponse[10];
	
	//  Closing PDP connection
	sim900_cmd("AT+CIPSHUT\r\n\0",simresponse);	
	if (strcmp(simresponse,"SHUT OK\0")!=0){  //  Previous PDP connection not closed properly
		return 0;
	}
	
	//  Reception Enable
	sbi(UCSR1B,RXEN1);
	
	//  Setting APN
	UART_1_puts("AT+CSTT=\"");  //  To assign the APN username and password
	UART_1_puts(apn);
	UART_1_puts("\"\,\"\"\,\"\"\r\n");
		
	// Getting response
	sim900_response(simresponse);
	
	if (strcmp(simresponse,"OK\0")==0){  //  All good
		return 1;
	}
	
	return 0;
}

uint8_t allocate_dynaIP(){
	char simresponse[20];
	
	//  To attach to GPRS connection
	sim900_cmd("AT+CIICR\r\n\0",simresponse);
	if (strcmp(simresponse,"OK\0")==0){
		//  To allocate dynamic IP
		sim900_cmd("AT+CIFSR\r\n\0",simresponse);
		if (strcmp(simresponse,"ERROR\0")!=0){
			return 1;
		}
	}
	return 0;
} 

uint8_t Log_data(char* responseimp,char* Serial_num,char* loc_lat,char* loc_long){
	char simresponse[512];
	uint8_t tabname_length=0,e=32,l=0,i=0;
	
	tabname_length=strlen(Serial_num);
	if (tabname_length!=13){
		while (l<13){
			Serial_num[i]=eeprom_read_byte((uint8_t*)e);
			i++;e++;l++;
		}
		Serial_num[i]='\0';
	}
	//  GPRS services connected and getting ready to Log
	sim900_cmd("AT+CIPSTART=\"TCP\"\,\"52.74.151.81\"\,\"80\"\r\n\0",simresponse);
	if (strcmp(simresponse,"OK\0")==0){
		
		//  Enabling data reception again to verify connection
		sbi(UCSR1B,RXEN1);
		
		//  Verifying connection
		sim900_response(simresponse);
		
		//  Start sending the data							
		UART_1_puts("AT+CIPSEND\r\n\0");
		_delay_ms(500);
		
		// Sending data
		UART_1_puts("GET /datalogging/write.php?tabname=\0");
 		UART_1_puts(Serial_num);
 		UART_1_putc('&');
		UART_1_puts(responseimp);
		UART_1_puts("&A08=");
		UART_1_puts(loc_lat);
		UART_1_puts("&A09=");
		UART_1_puts(loc_long);
		UART_1_puts(" HTTP/1.1\r\n\0");
		UART_1_puts("Host:52.74.151.81\r\nAccept: */*\r\nAccept-Language: en-us\r\nConnection: Keep-Alive\r\n\r\n\x1A\0");
		
		//  Enabling data reception
		sbi(UCSR1B,RXEN1);
				
		//  Confirm response from SIM900
		sim900_response(simresponse);
				
		return 1;
	}
	
	return 0;
}

void sim900_response(char* response){
	uint8_t i=0,dataarrived=0;
	char initial[]={'0','0'};
		
	while(!(initial[0]==0x0D && initial[1]==0x0A)){
		initial[0]=initial[1];
		dataarrived=wait_for_data();
		if (dataarrived==0){
			break;
		}
		initial[1]=UDR1;
	}

	while(1){		
		dataarrived=wait_for_data();
		if (dataarrived==0){
			break;
		}
		response[i]=UDR1;
		if(response[i]==0x0D){
			break;
		}
		i++;
	}

	//  Terminating string
	response[i]="\0";

	//  Reception Disable
	cbi(UCSR1B,RXEN1);
} 

uint8_t get_signalstrength(){
	char dummy[5],strength[3],simresponse[200];
	uint8_t signalstrength;
	
	//  To get signal strength
	sim900_cmd("AT+CSQ\r\n\0",simresponse); 
	
	//  Re-enabling reception for OK
	sbi(UCSR1B,RXEN1);
	
	sim900_response(dummy);
	
	strength[0]=simresponse[6];
	if (simresponse[7]!=','){
		strength[1]=simresponse[7];
		strength[2]='\0';
	} else {
		strength[1]='\0';
	}	
	
	signalstrength=atoi(strength);
	
	if (signalstrength<10 || signalstrength==99){
		return 0;
	} else if (signalstrength>=10 && signalstrength<20){
		return 1;
	} else if (signalstrength>=20 && signalstrength<30){
		return 2;
	} else if (signalstrength>30){
		return 3;
	}
}

ISR(TIMER1_OVF_vect)
{
	overflowcount++;
	
	if (overflowcount>=25){  //  To account for 50 secs
		overflowcount = 0;
		skip = 1;
		cbi(TCCR1B,CS12);
	}
}

void start_timer(){
	skip = 0;
	TCNT1 = 0;
	sbi(TCCR1B,CS12);
}

uint8_t wait_for_data(){
	// Start Timer
	start_timer();
	
	//  Waiting for data
	while (!(UCSR1A & (1<<RXC1))){
		_delay_us(10);
		if (skip==1){
			return 0;  //  No data; timer expired
		}
	}
	
	//  Stop Timer
	cbi(TCCR1B,CS12);
	
	return 1;  //  Data arrived
}

 void get_loc(char* apn,char* loc_lat,char* loc_long){
	char simresponse[70],set_apn_str[60],dummy[5];
	uint8_t i=0,p=0,number_attempts=0,allokay=1;
	
	//  Initializing Latitude and Longitude if fetch is unsuccessful
	loc_lat[0]='0';loc_lat[1]='\0';
	loc_long[0]='0';loc_long[1]='\0';
	
	//  No. of attempts
	number_attempts=NumberOfAttempts;
	
	while (number_attempts>0){
		number_attempts--;
		
		if (allokay==0){
			allokay=1;
			//  Deactivitate Bearer
			sim900_cmd("AT+SAPBR=0,1\r\n\0",simresponse);
			_delay_ms(500);  //  Waiting for response
		}
	
		//  Setting connection type as GPRS
		sim900_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n\0",simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			continue;
		}
	 
		//  Setting APN
		set_apn_str[0]='\0';
		strcat(set_apn_str,"AT+SAPBR=3,1,\"APN\",\"");
		strcat(set_apn_str,apn);
		strcat(set_apn_str,"\"\r\n\0");
		sim900_cmd(set_apn_str,simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			continue;
		}
	 
		//  Open Bearer
		sim900_cmd("AT+SAPBR=1,1\r\n\0",simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			allokay=0;
			continue;
		}
	
		//  Query Bearer
		sim900_cmd("AT+SAPBR=2,1\r\n\0",simresponse);
		if(strcmp(simresponse,"ERROR\0")==0){
			continue;
		}
		//  Enable reception again for OK
		sbi(UCSR1B,RXEN1);
		sim900_response(simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			allokay=0;
			continue;
		}
	
		//  Get Latitude and Longitude
		sim900_cmd("AT+CIPGSMLOC=1,1\r\n\0",simresponse);
	    if(strcmp(simresponse,"ERROR\0")==0){
			continue;
		}
		//  Enable reception again for OK
		sbi(UCSR1B,RXEN1);
		sim900_response(dummy);
		if (strcmp(dummy,"OK\0")!=0){
			allokay=0;
			continue;
		}
	 
		i=0;p=0;
		while (simresponse[i]!=0x2C && i<70){  //  0x2C IS FOR ,
			i++;
		}
		i++;
		while (simresponse[i]!=0x2C && i<70){
			loc_long[p]=simresponse[i];
			p++;i++;	 
		}
		loc_long[p]='\0';
		i++;
		p=0;
		while (simresponse[i]!=0x2C && i<70){
			loc_lat[p]=simresponse[i];
			p++;i++;
		}
		loc_lat[p]='\0';
	
		//  Deactivitate Bearer 
		sim900_cmd("AT+SAPBR=0,1\r\n\0",simresponse);
		if (strcmp(simresponse,"OK\0")==0){
			break;
		}
	}
 
	return;
	 
 }