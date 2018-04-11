
/*

A simple program to demonstrate the use of USART of AVR micro

*************************************************************

See: www.eXtremeElectronics.co.in for more info

Author : Avinash Gupta
E-Mail: me@avinashgupta.com
Date : 29 Dec 2008

Hardware:
   ATmega8 @ 16MHz

   Suitable level converter on RX/TX line
   Connected to PC via RS232
   PC Software :  Hyper terminal @ 19200 baud
               No Parity,1 Stop Bit,
               Flow Control = NONE

*/

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include "PORT.h"
#include "LCD.h"

//This function is used to initialize the USART
//at a given UBRR value
void USARTInit(uint16_t ubrr_value)
{

   //Set Baud rate

   UBRR1L = ubrr_value;
   UBRR1H = (ubrr_value>>8);

   /*Set Frame Format


   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8

   */

   UCSR1C=(3<<UCSZ0);


   //Enable The receiver and transmitter

   UCSR1B=(1<<RXEN)|(1<<TXEN);


}


//This function is used to read the available data
//from USART. This function will wait untill data is
//available.
char USARTReadChar()
{
   //Wait untill a data is available

   while(!(UCSR1A & (1<<RXC1)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR1;
}


//This fuction writes the given "data" to
//the USART which then transmit it via TX line
void USARTWriteChar(char data)
{
   //Wait untill the transmitter is ready

   while(!(UCSR1A & (1<<UDRE1)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR1=data;
}

void main()
{
int temp1,temp2,temp3;
DDRA=0xFF;
DDRD=0x08;
DDRC=0xFF;
DDRG=0xFF;
   //This DEMO program will demonstrate the use of simple

   //function for using the USART for data communication

   //Varriable Declaration
   char data='A';

   /*First Initialize the USART with baud rate = 19200bps
   
   for Baud rate = 19200bps

   UBRR value = 51

   */

   USARTInit(51);    //UBRR = 51
	lcd_init();
   //lcd_char('A');
   //Loop forever

      //Read data
      data=USARTReadChar();

      /* Now send the same data but but surround it in
      square bracket. For example if user sent 'a' our
      system will echo back '[a]'.

      */

      USARTWriteChar('[');
	  temp1=USARTReadChar();
      USARTWriteChar(data);
      temp2=USARTReadChar();
	  USARTWriteChar(']');
	  temp3=USARTReadChar();
	  
	  
	  
	  
	  //_delay_ms(1000);
	  //lcd_char(temp1);
	  //lcd_char(temp2);
	  //lcd_char(temp3);

   while(temp2=='A')
   {
   lcd_string("Awesome");
   }
   lcd_string("Fail");
}
