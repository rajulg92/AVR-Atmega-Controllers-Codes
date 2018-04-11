/*
 * Keypad16.c
 *
 * Created: 01-01-2014 18:56:01
 *  Author: Coder Rajat
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/eeprom.h>
#include "LCD.h"

/*int KEYPAD_DELAY = 700;
int UBRR_VALUE = 25;
char GetPressedKey();
void DisplayMenu();
void InitializeEEPROM();
void UART_INIT();
uint8_t GetReplyFromMaster();
void PostCommandToMaster(int);
void ConfigureLcd();
void UART_INIT()
{
	UBRRH = (unsigned char) (UBRR_VALUE >> 8);
	UBRRL = (unsigned char) UBRR_VALUE;
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= (1 << USBS) | (1 << UCSZ1) | (1 << UCSZ0);
	// PD0 - RX
	// PD1 - TX
}

uint8_t GetReplyFromMaster()
{

}

void PostCommandToMaster(int data)
{
}

int main(void)
{
	InitializeEEPROM();
	UART_INIT();
	
    lcd_init();
	DDRA = 0x00;		// SET AS INPUT
	PORTA = 0x0F;		// MAKE PULLS ON OF LAST FOUR PINS\
	
	char key;
	
	lcd_string("----WELCOME----");
	lcd_cmd(LINE2);
	lcd_string("TEAM ROBOCON");
	_delay_ms(2000);
	DisplayMenu();
	
	while(1)
	{
		key = GetPressedKey();
		if(key == 255)
			continue;
		else if (key == '2')
		{
			ConfigureLcd();
			DisplayMenu();
		}
		else if (key == '1')
		{
			Communication();
			DisplayMenu();
		}
		lcd_char(key);		
		_delay_ms(700);
	}
	
	while(1);
}

/************************************************************************/
/* FOR COMMUNICATION 
	SEND 200 FROM SLAVE AND WAIT FOR RESPONSE AS 100 */
/************************************************************************/
/*
void Communication()
{
	int ans = 0;
	char key;
	lcd_cmd(0x01);	//Clear LCD
	lcd_string("Initializing");
	lcd_cmd(LINE2);
	lcd_string("Sequence....");
	_delay_ms(1000);
	
	while(1)
	{
		key = GetPressedKey();
		if (key == '*')
		{
			return;
		}
		
		ans = GetReplyFromMaster();
		
		if (ans == 100)
		{
			lcd_cmd(0x01);	//Clear LCD
			lcd_string("Master Connected");
			DisplayCommunicationMenu();
			
			while(1)
			{
				key = GetPressedKey();
				if (key == '*')
					return;
				else if (key == '1')
				{
					UnitTesting();
					break;
				}
				else if (key == 2)
				{
					PathTesting();
					break;
				}
			}			
		}
	}
}

/****************************(UNIT TESTING MODULE)*************************/
/* RESPONSE REQUEST PROTOCOL FOR MASTER SLAVE
	SLAVE(REQUEST)		MASTER(RESPONSE)	Keypad Key		Testing Module Under Consideration
		10					50					0			- Immediately Stop all Module Testing
		11					51					1			- Right Motor
		12					52					2			- Left Motor
		13					53					3			- Left Arm
		14					54					4			- Right Arm
		15					55					5			- Disc
		16					56					6
		17					57					7
		18					58					8
		19					59					9			
		
		ACCEPTANCE OF ABOVE COMMANDS IS NOTED BY RESPONSE AS 150
		EXECUTION OF ABOVE COMMANDS IS REPORTED BY RESPONSE AS 220
	*/
/************************************************************************/
/*
void UnitTesting()
{
	char key;
	int reply;
	
	while(1)
	{
		key = GetPressedKey();
		if(key == '*')
			return;
		else if (key == 255 || key == '#')
			continue;
		
		PostCommandToMaster(key-46);		// Done Everything here do not change parameter
		reply = GetReplyFromMaster();
		if(reply == 150)					// Command Acceptance Confirmation
		{
			lcd_cmd(0x01);					//clear LCD
			lcd_string("RequestedAccepted");
			lcd_cmd(LINE2);
			lcd_string("Wait...");
			
			while(1)
			{
				key = GetPressedKey();
				if(key == '*')
					return;
					
				reply = GetReplyFromMaster();
				
				if(reply == 220)			// Command Execution Success
				{
					lcd_cmd(0x01);			//Clear LCD
					lcd_string("Executed");
					lcd_cmd(LINE2);
					lcd_string("Successfully");
					_delay_ms(500);
					return;
				}
			}
		}
	}
}


/****************************(PATH TESTING MODULE)*************************/
/* RESPONSE REQUEST PROTOCOL FOR MASTER SLAVE
	SLAVE(REQUEST)		MASTER(RESPONSE)	Keypad Key		Testing Module Under Consideration
		30					60					0			- Path 1
		31					61					1			- Path 2
		32					62					2			- Path 3
		33					63					3			- Path 4
		34					64					4			- Path 5
		35					65					5			- Path 6
		36					66					6
		37					67					7
		38					68					8
		39					69					9			
		
		ACCEPTANCE OF ABOVE COMMANDS IS NOTED BY RESPONSE AS 150
		EXECUTION OF ABOVE COMMANDS IS REPORTED BY RESPONSE AS 220
	*/
/************************************************************************/
/*
void PathTesting()
{
		char key;
		int reply;
		
		while(1)
		{
			key = GetPressedKey();
			if(key == '*')
			return;
			else if (key == 255 || key == '#')
			continue;
			
			PostCommandToMaster(key-16);		// Done Everything here do not change parameter
			reply = GetReplyFromMaster();
			if(reply == 150)					// Command Acceptance Confirmation
			{
				lcd_cmd(0x01);					//clear LCD
				lcd_string("RequestedAccepted");
				lcd_cmd(LINE2);
				lcd_string("Wait...");
				
				while(1)
				{
					key = GetPressedKey();
					if(key == '*')
					return;
					
					reply = GetReplyFromMaster();
					
					if(reply == 220)			// Command Execution Success
					{
						lcd_cmd(0x01);			//Clear LCD
						lcd_string("Executed");
						lcd_cmd(LINE2);
						lcd_string("Successfully");
						_delay_ms(500);
						return;
					}
				}
			}
		}
}

void DisplayCommunicationMenu()
{
	lcd_cmd(0x01);		//Clear LCD
	lcd_string("1. Unit Testing");
	lcd_cmd(LINE2);
	lcd_string("2. Path Testing");
}


void ConfigureLcd()
{
	lcd_cmd(0x01);	//Clear LCD
	lcd_cmd(LINE1);
	lcd_string("1. Delay");
	
	char key = 0;
	
	while(1)
	{
			key = GetPressedKey();
			if (key == 255)
			{
				continue;
			}
			else if (key == '1')
			{
				lcd_cmd(0x01);	//Clear LCD
				lcd_string("Current Delay:");
				lcd_showvalue(KEYPAD_DELAY);
				lcd_cmd(LINE2);
				lcd_string("New Delay:");
				
				while(1)
				{
					key = GetPressedKey();
					if(key == '*')
						return;
					else if (key >= 48 && key <= 57)
					{
						if (key == '0')
						{
							KEYPAD_DELAY = 1000;
						}
						else
						{
							KEYPAD_DELAY = (key-48)*100;						
						}
						eeprom_update_byte(0x01,KEYPAD_DELAY);
						lcd_cmd(0x01);		// CLEAR LCD
						lcd_string("DELAY CHANGED");
						lcd_cmd(LINE2);
						lcd_string("NEW DELAY:");
						lcd_showvalue(KEYPAD_DELAY);
						_delay_ms(2000);
						return;
					}
				}
			}
			else if (key == '*')
			{
				return;
			}
	}	
}

void DisplayMenu()
{
	lcd_cmd(0x01);		// Clear LCD
	lcd_cmd(LINE1);
	lcd_string("1. Communication");
	lcd_cmd(LINE2);
	lcd_string("2. Configure LCD");
}

char GetPressedKey()
{
	char key = WhichKeyIsPressed();
	
	switch(key)
	{	
		case '1':
		return '9';
		break;
		
		case '2':
		return '8';
		break;
		
		case '3':
		return '7';
		break;
		
		case '4':
		return '6';
		break;
		
		case '5':
		return '5';
		break;
		
		case '6':
		return '4';
		break;
		
		case '7':
		return '3';
		break;
		
		case '8':
		return '2';
		break;
		
		case '9':
		return '1';
		break;
		
		case '0':
		return '*';
		break;
		
		case '/':
		return '0';
		break;
		
		case '.':
		return '#';
		break;
	}
}

int WhichKeyIsPressed()
{
	uint8_t r,c;
	PORTA|= 0X0F;

	for(c=0;c<3;c++)
	{
		 DDRA&=~(0X7F);
		DDRA|=(0X40>>c);
		for(r=0;r<4;r++)
		{
			if(!(PINA & (0X08>>r)))
			{
			return (r*3+c+46);
			}
		}
	}
	return 0XFF;//Indicate No key pressed
}

/************************************************************************/
	/* Address 0x01 - LCD DELAY VALUE : '0' - '9'	i.e. 48 - 57 in uint_8 format
										'0' : 10 x 100ms
										'1' : 1 x 100ms
										'2' : 2 x 100ms
										. . .  . .
										'9' : 9x100ms
										
					if(at Address 0x01 ANY VALUE ACCEPT THIS IS FOUND THEN INITIALIZE DELAY BY 500ms i.e. 6 at 0x01 )
					*/
	/************************************************************************/
/*void InitializeEEPROM()
{
	int D;
	
	char byte = eeprom_read_byte(0x01);
	if(byte >= 48 && byte <= 57)
	{
		if (byte == '0')
		{
			KEYPAD_DELAY = 1000;
			return;
		}
		KEYPAD_DELAY = (byte - 48) * 100;
		return;
	}		
	else
		eeprom_update_byte(0x01,'6');
}*/

int main()
{
	lcd_init();
	lcd_string("DISPLAY");
	lcd_gotoxy2(0);
	lcd_string("WORKING");
	while(1)
	{
		
	}
}