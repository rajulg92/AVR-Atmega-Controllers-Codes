

/*
 *	This file is to be included in your Master processor's program
 *	This header transfers the data to be displayed to supporting chip
 *	Make Changes in USARTINIT() function accordingly to ur main processor and set baud rate to 9600
 *	First call USARTINIT() in your main function
 *	To Display int, float, char, string just use the function
 *	lcd_display(Things that you wanna display)
 *	Example
 *	lcd_dispaly(32);			//int
 *	lcd_display((int)(x));		//int
 *	lcd_display('a');			//char
 *	lcd_display("string");		//string
 *	lcd_display(32.32);			//float
 *	lcd_display((float)(x));	//float
*/


#include<avr/io.h>
#include<util/delay.h>


void UartInit(void)
{
	DDRH |= (1<<PH1);
	DDRH &= ~(1<<PH0);
	
	UCSR2A |= (1<<U2X2);
	UCSR2B |= (1<<RXEN2) | (1<<TXEN2);
	UCSR2C |=  (1 << UCSZ21)| (1<<UCSZ20) | (1<<USBS2);
	UBRR2 = 12;
}

void UartTransmit(uint8_t data)
{
	while ( !( UCSR2A & (1<<UDRE2)) );
	UDR2 = data;
}

uint8_t UartReceive(void)
{
	while ( !( UCSR2A & (1<<RXC2)) );
	return UDR2;
}




void lcd_display(long int num)
{
	unsigned char H=0;
	int n = 0,flag = 0;
	
	if(num == 0)
	{
		flag = 0;
	}
	else if(num%10000 == 0)
	{
		flag = 4;
	}

	else if(num%1000 == 0)
	{
		flag = 3;
	}

	else if(num%100 == 0)
	{
		flag = 2;
	}

	else if(num%10 == 0)
	{
		flag = 1;
	}

	else
	{
		flag = 0;
	}


	while(num>0)
	{
		H = num%10;
		num = num/10;
		n = (n*10)+H;
	}
	
	while(n>0)
	{
		
		H = n%10;
		n = n/10;
		UartTransmit(H+48);
		lcd_showvalue(H+48);
	}
	
	
	while(flag > 0)
	{
		UartTransmit('0');
		flag--;
	}
}

/*void lcd_display(double a)
{
	lcd_showvaluef(a);
}


void lcd_display(char a[32])
{
	int i = 0;
	
	while(a[i] != '\0')
	{
		lcd_char(a[i]);
		i++;
		if(i == 16)
		{
			lcd_cmd(LINE2);
		}
	}
}

void lcd_display(char a)
{
	lcd_char(a);
}*/