/*
 * DataLogging_SMS_with_Auto_Reset.c
 *
 * Created: 29-02-2016 18:26:33
 *  Author: RajulGupta
 */ 
/********************Code Summary***************************************/
/*
1> The following code is used for dataLogging in Cold Storage only.
2> This code also releases the DataFreezing issue by resetting the PCB stack with the help of small datafreeze PCB
3> For Enabling the DataLogging on any serial no. send SMS via Master Mobile no. i.e. +919021025039 
in a pattern "SNO+<Serial no>" for example if serial no. is E011501A999 the we'll send the SMS as SNO+E011501A999
from the Master no to the SIM Card no. installed in that system.
4> To register Customer no. so that the Customer will be able to ON/OFF the system please send +91<customer no> i.e. 
if customer no is 8605029237 then send SMS as +918605029237 from the Master mobile no to the SIM card no. installed in that system.
After registering the no. now only Master and Customer will be able to ON/OFF system.
5> To ON/OFF system send SMS ON or OFF(regarless of upper case and lower case) from Master and Customer Mobile no.
6> Also this code enables the DataFreeze Algo only after 1 hr of Starting the system.So, after 1 hr if dataFreezes 
then system will restart in every 15 mins.
7> To bypass dataFreeze Algo just switch the system Remotely OFF from the Portal.
8> To again start the Datafreeze Algo switch the system Remotely ON from the Portal.
9>Fuse bits used should be E2 D9
*/ 


#define F_CPU 8000000UL		//freq 8MHz

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
#define MASTER_NUMBER "+919021025039\0"

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define NumberOfAttempts 6

//  Global variables for Timer
uint8_t skip=0,overflowcount=0;


//Global Variable for DataFreeze Reset
uint8_t counter_datafreeze=0,flag_datafreeze=0,counter_main=0;
char str_datafreeze_main[100],str_datafreeze[100];

//  Function Definition 
uint8_t get_apn(char* apn,char* user);
void sim900_cmd(char* str,char* response);
uint8_t get_sms(uint8_t* system_on,char* customer_no,char* Serial_num);
uint8_t dataLog(uint8_t system_on,char* apn,char* Serial_num,uint8_t signal_strength,char* user,char* loc_lat,char* loc_long,uint8_t flag_cooldown);
uint8_t fetch_gprs();
void get_data(uint8_t system_on,char* responseimp,uint8_t  signal_status,uint8_t signal_strength,char* user,uint8_t flag_cooldown);
uint8_t set_apn(char* apn);
uint8_t allocate_dynaIP();
uint8_t Log_data(char* responseimp,char* Serial_num,char* loc_lat,char* loc_long);
void sim900_response(char* response);
uint8_t get_signalstrength();
void start_timer();
uint8_t wait_for_data();
void get_loc(char* apn,char* loc_lat,char* loc_long);
void reset_SIM900();


void main(){
	
	DDRD = 0xFA;
	DDRB = 0xFF;
	
	// To prevent timer from triggering
	sbi(PORTB,6);
	
	// Delay for SIM900 initialization <- To investigate why we need this delay here
	_delay_ms(20000);
	
	uint8_t flag_get_apn=0,flag_get_sms=0,system_on,signal_strength=0,flag_datalog=0,flag_cooldown=0x89;
	uint8_t dummy,data_counter=0;
	uint16_t e=0,l=0,i=0;
	char Serial_num[20],customer_no[20],apn[30],user[30],responseimp[512],loc_lat[15],loc_long[15];
		
	// Rg Making SIM900 Power ON
	sbi(PORTB,7);
	
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
	_delay_ms(5);
	
	//  Reception Enable 
	sbi(UCSR0B,RXEN0);
	
	// Start Timer
	start_timer();

	//  Stop Timer
	cbi(TCCR1B,CS12);

	// To read last updated Serial No. of controller
	e=32;l=0;i=0;
	while (l<11){
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

	while(1){
		
		//Initial 1 Hr period will be cooldown period before starting the Datafreeze Algo
		counter_main++;
		if (counter_main>45){
			flag_cooldown=0x88;
			counter_main=50;
			}else{
			flag_cooldown=0x89;
		}
		
		// To fetch APN
		while (flag_get_apn!=1){
			flag_get_apn = get_apn(apn,user);
			data_counter++;
			if(data_counter>10){
				// Acquiring data for resetting
				get_data(system_on,responseimp,2,signal_strength,user,&flag_cooldown);
				data_counter=0;
			
			}
		}
		
		//  Get Location
		get_loc(apn,loc_lat,loc_long);
		
		//  To get signal strength
		signal_strength=get_signalstrength();
		
		// To fetch SMS 	
		data_counter=NumberOfAttempts; // Number of attempts to be made if SMS fetch fails
		while (data_counter!=0){
			flag_get_sms = get_sms(&system_on,customer_no,Serial_num);
			data_counter--;
			if(flag_get_sms==1){  // SMS Fetch successful
				break;
			}
		}
		
 		// Logging Data
		flag_datalog = dataLog(system_on,apn,Serial_num,signal_strength,user,loc_lat,loc_long,flag_cooldown);
		if(flag_datalog==0){
			reset_SIM900();  //  Reset SIM900 to get CGATT=1 sooner
			flag_get_apn=0;  //  Getting APN again since we're resetting the chip
		}
		
	}
}
 
uint8_t get_apn(char* apn,char* user){
	 
	uint8_t i=0,p=0;
	char simresponse[200];
	
	UART_0_puts("Checking SIM...\r\n");
	// SIM900 Echo disabled in response
	UART_1_puts("ATE0\r\n\0");  
	_delay_ms(500);
	
	// SIM900 module check
	sim900_cmd("AT\r\n\0",simresponse);
	if (strcmp(simresponse,"OK\0")!=0){
		UART_0_puts("No Response...\r\n");
		UART_0_puts("Rechecking...\r\n");
		return 0;
	}
	UART_0_puts("SIM Checked OK...\r\n");
	UART_0_puts("Checking SimCard...\r\n");
	
	// To check if SIM card is present	 
	sim900_cmd("AT+CSMINS?\r\n\0",simresponse); 
	if (strcmp(simresponse,"+CSMINS: 0\,1\0")!=0){
		UART_0_puts("No Response...\r\n");
		UART_0_puts("Rechecking...\r\n");
		return 0;
	}
	UART_0_puts("SimCard Present...\r\n");
	UART_0_puts("Checking SIMCARD Registration...\r\n");
	
	// To check if SIM card is registered
	sim900_cmd("AT+CREG?\r\n\0",simresponse);  
	if (!((strcmp(simresponse,"+CREG: 0\,1\0")==0) || (strcmp(simresponse,"+CREG: 0\,5\0")==0))){
		UART_0_puts("No Response...\r\n");
		UART_0_puts("Rechecking...\r\n");
		return 0;
	}
	UART_0_puts("SimCard Registered OK...\r\n");
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
	UART_0_puts("SIM network provider...");
	UART_0_puts(user);
	UART_0_puts("\r\n");
	
	//APN Compare and Assignment
	if (strcmp(user,"Hutch\0")==0){
		strcpy(apn,"www");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if (strcmp(user,"Vodafone\0")==0){
		strcpy(apn,"www");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if ((strcmp(user,"airtel\0")==0) || (strcmp(user,"Airtel\0")==0)){
		strcpy(apn,"airtelgprs.com");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if(strcmp(user,"CellOne\0")==0){
		strcpy(apn,"bsnlenet");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;;
	}
	else if(strcmp(user,"Reliance\0")==0){
		strcpy(apn,"rcomwap");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if(strcmp(user,"TATA\0")==0){
		strcpy(apn,"tata.docomo.internet");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if(strcmp(user,"Uninor\0")==0){
		strcpy(apn,"uninor");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1;
	}
	else if(strcmp(user,"!dea\0")==0){
		strcpy(apn,"internet");
		UART_0_puts("APN...");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		return 1; 
	}
	else{
		UART_0_puts("APN not found...\r\n");
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

uint8_t get_sms(uint8_t* system_on,char* customer_no,char* Serial_num){
	
	uint8_t i=0,p=0,dataarrived=0;
	uint8_t update_customer_no=0,update_serial_no=0,update_remote_control=0;
	int no_of_msg,message_number;
	char no_msg[6],response_number[150],contact_no[20],simresponse[30],msg[160],dummy[5];
	char initial[]={'0','0'};
	
	UART_0_puts("Checking no. of unread messages...\r\n");
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
	UART_0_puts("no. of unread messages are..");
	UART_0_puts(no_msg);
	UART_0_puts("\r\n");
	
	// Exiting the function if no new messages have arrived
	if (no_of_msg==0){
		return 1;
	}
	
	// Changing the message to readable format
	sim900_cmd("AT+CMGF=1\r\n",simresponse);
	if (strcmp(simresponse,"OK\0")!=0){
		return 0;
	}
	UART_0_puts("Messages converted to Readable Format...\r\n");
	
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
		
		// If message doesn't match authenticated source, move to next message
		if (!((strcmp(contact_no,customer_no)==0) || (strcmp(contact_no,MASTER_NUMBER)==0))){
			message_number++;
			continue;
		}
		
		if ((msg[0]=='+') && (strcmp(contact_no,MASTER_NUMBER)==0)){  //  Changing customer number by Master
			strcpy(customer_no,msg);
			UART_0_puts("Customer No. changed to ");
			UART_0_puts(customer_no);
			UART_0_puts("\r\n");
			update_customer_no=1;
		}
		else if ((strcasecmp(msg,"on")==0) && ((strcmp(contact_no,MASTER_NUMBER)==0) || (strcmp(contact_no,customer_no)==0))){  //  Commanding system to Switch on by Customer/Master
			*system_on=0x31;
			UART_0_puts("System Turned ON....\r\n");
			sbi(PORTB,7);
			update_remote_control=1;
		}
		else if((strcasecmp(msg,"off")==0) && ((strcmp(contact_no,MASTER_NUMBER)==0) || (strcmp(contact_no,customer_no)==0))){  //  Commanding system to Switch off by Customer/Master
			*system_on=0x30;
			UART_0_puts("System Turned OFF....\r\n");
			cbi(PORTB,7);
			update_remote_control=1;
		}
		else if ((strncmp(msg,"SNO+E",5)==0) && (strcmp(contact_no,MASTER_NUMBER)==0)){  //  To change serial number by Master
			strncpy(Serial_num,msg+4,11);
			
			UART_0_puts("Serial No. changed to ");
			UART_0_puts(Serial_num);
			UART_0_puts("\r\n");
			update_serial_no=1;
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
		while (i<=10){
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
	UART_0_puts("Deleting Read messages...\r\n");
	
	return 1;
}

uint8_t dataLog(uint8_t system_on,char* apn,char* Serial_num,uint8_t signal_strength,char* user,char* loc_lat,char* loc_long,uint8_t flag_cooldown){
	uint8_t flag_fetch_gprs=0,flag_set_apn=0,flag_allocate_dynaIP=0,flag_Log_data=0,flag_get_apn=0;
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
			return 0;
		}	
	}
	
	//For Testing
	//strcpy(responseimp,"A01=1&A02=2");
	
	// Acquiring data for logging
	get_data(system_on,responseimp,2,signal_strength,user,flag_cooldown);
	// Acquiring data for logging and setting APN
	data_counter=NumberOfAttempts;  // Number of attempts to be made if APN set fails
	while (data_counter!=0){
		//Setting APN
		flag_set_apn = set_apn(apn);
		data_counter--;
		if(flag_set_apn==1){  // APN set successful
			break;
		}
		else if(data_counter==0){  // Number of attempts exhausted
			return 0;
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
			return 1;
		}		
	}
	
	return 1;
}

uint8_t fetch_gprs(){
	char simresponse[20];
	UART_0_puts("Searching GPRS...\r\n");
	
	sim900_cmd("AT+CGATT?\r\n",simresponse);
	if(strcmp(simresponse,"+CGATT: 0\0")==0){  // If GPRS is not attached
		UART_0_puts("GPRS returns ERROR...\r\n");
		_delay_ms(5000);
	}else if(strcmp(simresponse,"+CGATT: 1\0")==0){  // GPRS is attached properly
		//  Reception Enable again for OK
		sbi(UCSR1B,RXEN1);
		UART_0_puts("GPRS OK...\r\n");
		sim900_response(simresponse);
		return 1;
	}
	return 0;
}

void get_data(uint8_t system_on, char* responseimp,uint8_t  signal_status, uint8_t signal_strength,char* user,uint8_t flag_cooldown){
	
	int t=0,takedata=0,dataarrived=0;
	char dataimp,ch1[3],ch2[3];
	
	UART_0_puts("Fetching DATA...\r\n");
	//  Reception Enable
	sbi(UCSR0B,RXEN0);
	
	while(1){
		while(!((UCSR0A) & (1<<RXC0)));  // Wait while data is being received
		dataimp = UDR0;
		
		// If last character # is received..
		if (((dataimp == 0x23) && (takedata == 1)) || t>=511){  //  #=0x23
			//  Reception Disable
			cbi(UCSR0B,RXEN0);
			responseimp[t]='\0';
			
			takedata = 0;
			if (strlen(responseimp)<10){
				get_data(&system_on,responseimp,&signal_status,&signal_strength,user,&flag_cooldown);
				break;
			}
			if (flag_datafreeze==1 && flag_cooldown==0x88){
				//Copying 68 bytes of responseimp to dataFreeze String for Auto Reset
				strncpy(str_datafreeze_main,str_datafreeze,68);
				strncpy(str_datafreeze,responseimp,68);
				
				UART_0_puts(str_datafreeze_main);
				UART_0_puts("\r\n");
				UART_0_puts(str_datafreeze);
				UART_0_puts("\r\n");
				UART_0_puts("COunter =");
				UART_0_putc(counter_datafreeze+48);
				UART_0_puts("\r\n");
				_delay_ms(2000);
	
				if (strcmp(str_datafreeze_main,str_datafreeze)==0){
					counter_datafreeze++;
					}else{
					counter_datafreeze=0;
				}
				UART_0_puts("Data Recieved...\r\n");
				if (counter_datafreeze>8){// 8 for reset after 10 data points
					UART_0_puts("RESETTING...\r\n");
					_delay_ms(5000);
					counter_datafreeze=0;
					//Reset the PCB Stack
					cbi(PORTB,6);
				}
				
			}
			
			break;
		}
		// Continue taking data
		if (takedata==1){
			responseimp[t]=dataimp;
			t++;
		}
		// If first character $ is received, start taking data..
		if ((dataimp == 0x24) && (takedata == 0)){  //  $=0x24
			takedata=1;
			t=0;
		}
	}
}

uint8_t set_apn(char* apn){
	char simresponse[10];
	uint8_t flag_allocate_dynaIP=0;
	
	UART_0_puts("Closing PDP Settings...\r\n");
	//  Closing PDP connection
	sim900_cmd("AT+CIPSHUT\r\n\0",simresponse);	
	if (strcmp(simresponse,"SHUT OK\0")!=0){  //  Previous PDP connection not closed properly
		UART_0_puts("PDP connection not closed properly...\r\n");
		UART_0_puts("Re-shutting...\r\n");
		return 0;
	}
	UART_0_puts("PDP Settings closed Properly...\r\n");
	UART_0_puts("Setting apn to ");
	UART_0_puts(apn);
	UART_0_puts("\r\n");
	
	//  Reception Enable
	sbi(UCSR1B,RXEN1);
	
	//  Setting APN
	UART_1_puts("AT+CSTT=\"");  //  To assign the APN username and password
	UART_1_puts(apn);
	UART_1_puts("\"\,\"\"\,\"\"\r\n");
		
	// Getting response
	sim900_response(simresponse);
	
	if (strcmp(simresponse,"OK\0")!=0){  //  Not good
		UART_0_puts("APN not set properly...\r\n");
		UART_0_puts("Re-setting APN...\r\n");
		return 0;
	}
	UART_0_puts("APN set properly...\r\n");
	UART_0_puts("Allocating Dynamic IP...\r\n");
	// Allocating dynamic IP
	flag_allocate_dynaIP = allocate_dynaIP();
	if(flag_allocate_dynaIP==1){  // Dynamic IP allocation successful
		return 1;
	}
	UART_0_puts("Dynamic IP Allocated successfully...\r\n");
		
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
	char simresponse[512],var_remote,dummy;
	uint8_t tabname_length=0,dataarrived=0,e=32,l=0,i=0;
	
	//  If Serial_num gets erased mysteriously
	tabname_length=strlen(Serial_num);
	if (tabname_length!=11){
		while (l<11){
			Serial_num[i]=eeprom_read_byte((uint8_t*)e);
			i++;e++;l++;
		}
		Serial_num[i]='\0';
	}
	UART_0_puts("Starting CHIP...\r\n");
	
	//  GPRS services connected and getting ready to Log
	sim900_cmd("AT+CIPSTART=\"TCP\"\,\"52.74.151.81\"\,\"80\"\r\n\0",simresponse);
	if (strcmp(simresponse,"OK\0")==0){
		
		//  Enabling data reception again to verify connection
		sbi(UCSR1B,RXEN1);
		
		//  Verifying connection
		sim900_response(simresponse);
		UART_0_puts("Sending Data...\r\n");
		
		//  Start sending the data							
		UART_1_puts("AT+CIPSEND\r\n\0");
		_delay_ms(500);

		// Sending data
		UART_1_puts("GET /datalogging/write.php?tabname=E011501A005&\0");
 	//	UART_1_puts(Serial_num);
 	//	UART_1_putc('&');
		UART_1_puts(responseimp);
		
		UART_1_puts("&A46=");
 		UART_1_puts(loc_lat);
 		UART_1_puts("&A47=");
 		UART_1_puts(loc_long);
		UART_1_puts(" HTTP/1.1\r\n\0");
		UART_1_puts("Host:52.74.151.81\r\nAccept: */*\r\nAccept-Language: en-us\r\nConnection: Keep-Alive\r\n\r\n\x1A\0");
		
		//  Enabling data reception
		sbi(UCSR1B,RXEN1);
		
		//  Disabling data reception UART0
		cbi(UCSR0B,RXEN0);
		
		while (dummy!='$'){
			dataarrived=wait_for_data();
			if (dataarrived==0){
				break;
			}
			dummy=UDR1;
		}
		dataarrived=wait_for_data();
		var_remote=UDR1;
		
		//  Enabling data reception UART0
		sbi(UCSR0B,RXEN0);
		if (dummy=='$'){
			UART_0_puts("Data Sent Successfully to ");
			UART_0_puts(Serial_num);
			UART_0_puts("\r\n");
		}
		else{
			UART_0_puts("Data Sent returns ERROR...\r\n");
			UART_0_puts("Check String or SIMCARD balance...\r\n");
		}
		if (var_remote=='1'){
			cli();
			flag_datafreeze=1;
			sei();
		}
		else if(var_remote=='2'){
			cli();
			flag_datafreeze=0;
			sei();
		}
				
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
			UART_0_puts("No Response...\r\n");
			UART_0_puts("Re-Checking...\r\n");
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
	
	if (overflowcount>=10){  //  To account for 20 secs
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

void reset_SIM900(){
	
	char simresponse[20];
	
	sim900_cmd("AT+CPOWD=1\r\n\0",simresponse);	
	_delay_ms(20000);
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
	number_attempts=NumberOfAttempts-4;  //  Because some ICs don't have this functionality so reducing number of attempts to save time
	
	while (number_attempts>0){
		number_attempts--;
		
		if (allokay==0){
			allokay=1;
			UART_0_puts("Deactivating Bearer...\r\n");
			//  Deactivitate Bearer
			sim900_cmd("AT+SAPBR=0,1\r\n\0",simresponse);
			_delay_ms(500);  //  Waiting for response
		}
		UART_0_puts("setting Connection type as GPRS...\r\n");
		//  Setting connection type as GPRS
		sim900_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n\0",simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			UART_0_puts("Response returns ERROR...\r\n");
			continue;
		}
		UART_0_puts("Connection Type set to GPRS successfully...\r\n");
		UART_0_puts("Setting APN for Loc....\r\n");
		//  Setting APN
		set_apn_str[0]='\0';
		strcat(set_apn_str,"AT+SAPBR=3,1,\"APN\",\"");
		strcat(set_apn_str,apn);
		strcat(set_apn_str,"\"\r\n\0");
		sim900_cmd(set_apn_str,simresponse);
		if (strcmp(simresponse,"OK\0")!=0){
			UART_0_puts("APN not set...returns ERROR...\r\n");
			continue;
		}
		UART_0_puts("APN set successfully to ");
		UART_0_puts(apn);
		UART_0_puts("\r\n");
		
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
		UART_0_puts("Fetching Location Coordinates...\r\n");
		//  Get Latitude and Longitude
		sim900_cmd("AT+CIPGSMLOC=1,1\r\n\0",simresponse);
	    if(strcmp(simresponse,"ERROR\0")==0){
			UART_0_puts("Location Not found....Retrying...\r\n");
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
		UART_0_puts("Location Fetched Successfully...\r\n");
		UART_0_puts("Latitude=");
		UART_0_puts(loc_lat);
		UART_0_puts("\r\n");
		UART_0_puts("Longitude=");
		UART_0_puts(loc_long);
		UART_0_puts("\r\n");
		//  Deactivitate Bearer 
		sim900_cmd("AT+SAPBR=0,1\r\n\0",simresponse);
		if (strcmp(simresponse,"OK\0")==0){
			break;
		}
	}
 
	return;
	 
 }