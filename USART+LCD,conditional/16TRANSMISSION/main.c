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
#include<reg51.h>
#include<reg51.h>
#include "LCD.h"
 

#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;
#define CR 0X0A
#define LF 0X0D
#define EOM 0X1A




void RX_REPLY_2_SEND_MSG();
void SEND_CMD(char *BASE_ADD,char COUNT);
void SEND_CRLF(char);
unsigned char recv_byte(void);
void READ_SMS(void);
void DELETE_SMS(void);
void RX_REPLY();
void ENTER(void);
void DELAY();

void SEND_MSG(char *,char);
void RX_REPLY_2_SEND_MSG();

void busy();                          //LCD busy
void CMD_WRT(char);           //To write the commands to the LCD
void DATA_WRT(char);         //To write the data to the LCD
void LCD_WRT(char *);
void DELAY();
void TRANSMIT(char *);
void transmit_byte(char );
char SCI_ReceiveByte( void );
char byt,i;

char flag12=0,count12,message[50];

 void main()
  {
    char CMD[]={0x38,0x0f,0x01,0x06,0x80},TEMP1,a, next=0;
    
    char code CMD_1[]="AT";
    
    char code CMD_3[]="AT+CMGF=1";
    char code CMD_4[]="AT+CMGD=1";
    char code MSG[]="MESSAGE RECIVED";



    for(i=0;i<5;i++)
    {
    TEMP1=CMD[i];                         //write the commands to the LCD
    CMD_WRT(TEMP1);
    }

    TMOD=0X20;                              //TIMER 1 , MODE 2
    TH1=0XFD;                              //9600 BAURD RATE            //SERAIL INIT
    SCON=0X50;
    TR1=1;                                  //TIMER1 ON
    DELAY();
    DELAY();
    DELAY();                               //DELAY
    DELAY();

 
    
    CMD_WRT(0X01);
          CMD_WRT(0X80);
 LCD_WRT("  welcome");
 
      DELAY();
       DELAY();
       SEND_CMD(CMD_1,2);
     ENTER();
    RX_REPLY();

    DELAY();

    SEND_CMD(CMD_3,9);
    ENTER();
    RX_REPLY();

    DELAY();


       
 while(1)
 {           
 
        
        DELAY();
        
        SEND_CMD(CMD_4,9);        // DELET THE MSSG
        ENTER();    
        RX_REPLY();    
        DELAY();       
        DELAY();
        
        DELAY();
        
        recv_byte();
        recv_byte();
        
        DELAY();
        DELAY();
        
        READ_SMS();                       // READ THE MSSG
        
        DELAY();
        DELAY();
        
        DELAY();
        CMD_WRT(0X01);
        CMD_WRT(0X80);
        DELAY();
        DELAY();
        
        
        
        CMD_WRT(0X01);
        CMD_WRT(0X80);
        
        for(a=0;a<count12;a++)
        {
        DATA_WRT(message[a]);
        next++;
        if(next==0X0f)
        {                                  // DISPLAY MSG IN LCD
        next=0;
        CMD_WRT(0XC0);
        }
        
        }
        next=0;

          DELAY();
        DELAY();

       SEND_MSG(MSG,15);                  //SEND THE MSG
           DELAY();
        DELAY();
            DELAY();
        DELAY();

 }
 }
    

    
    void busy()
    {
    D7=1;
    rs=0;                             //cmd mode
    rw=1;                             //read
    while(D7!=0)                    //wait till LCD is ready
    {
    en=0;
    en=1;                            // low to high latch
    }
    }
    
    
    
    void CMD_WRT(unsigned char val)
    {
    busy();
    LCD_PORT=val;                   //write cmd to P2
    rs=0;                           //cmd mode
    rw=0;                           //write
    en=1;
    en=0;                           //high to low latch
    }
    
    
    void DATA_WRT(unsigned char ch)
    {
    busy();
    LCD_PORT = ch;                  //write cmd to P2
    rs=1;                          //data mode
    rw=0;                          //write
    en=1;
    en=0;                          //high to low latch
    }
    
    void LCD_WRT(unsigned char *string)
{
    while(*string)
        DATA_WRT(*string++);
}
/*    void transmit_byte(unsigned char  byte)       // TRANSIMITT SERIAL DATA
    {
    SBUF=byte;                                  //TRANSMIT DATA FROM  BYTE TO SBUF
    while(!TI);                                   // WAIT TILL DTA GET TRANSMITTED
    TI=0;
    }
    
    void TRANSMIT(unsigned char *string)
    {
    while(*string)
    transmit_byte(*string++);
    }
    
    unsigned char SCI_ReceiveByte( void )
    {                                            // RECIVING SERIAL DATA
    unsigned char byte;
    //    RI=0;
    while(RI!=1);                                // WAIT TILL RI IS HIGH
    byte = SBUF;                                //RECIVE DATA FROM SBUF TO BYTE
    RI=0;
    return byte;                                 // RETURN THE DATA
    }
     */
void DELAY()
{
    unsigned int X=6000000,Y=60000000;
    while(X--);
    while(Y--);
}

 void ENTER(void)    
 {
    SEND_CRLF(CR);
    SEND_CRLF(LF);
 }

 void SEND_CMD(unsigned char *BASE_ADD,unsigned char COUNT)
 {
    unsigned char I;
    for(I=0;I<COUNT;I++)
    {
      SBUF=*BASE_ADD;
      while(TI!=1);
      TI=0;
      BASE_ADD++;
    }
 }


 void RX_REPLY()
 {
    unsigned char RX=0;
    while(RX!='O')
    {
      while(RI!=1);
      RX=SBUF;
      RI=0;
    
    }
 }

 void SEND_CRLF(unsigned char CRLF)
 {
   SBUF=CRLF;
   while(TI!=1);
   TI=0;    
 }

void READ_SMS(void)
 {
  unsigned char code CMD_5[]="AT+CMGR=1";
  unsigned char byte,i=0, flag=0;
          CMD_WRT(0X01);
          CMD_WRT(0X80);
   SEND_CMD(CMD_5,9);
   ENTER();
    
    while(byte != CR)
    {    
     byte=recv_byte();        
    }    
    
    while(byte != LF)
    {    
     byte=recv_byte();    
    }
        
       while(byte != CR)
    {    
     byte=recv_byte();        
    }

    while(byte != LF)
    {    
     byte=recv_byte();    
    }    


     while(byte != LF)
    {    
     byte=recv_byte();
     message[count12]= byte;
     count12++;    
    
    }    




          
  }

 unsigned char recv_byte()
  {
    unsigned char c;
    while(RI!=1);
    c = SBUF;                        
    RI = 0;
    return(c);
  }
 
   void SEND_MSG(unsigned char *BASE_ADDR,unsigned char COUNT)
{
       unsigned char code CMD_9[]="AT+CMGS=\"1234567890\"";
    unsigned char I;
    SEND_CMD(CMD_9,20);
    ENTER();


    RX_REPLY_2_SEND_MSG();

    for(I=0;I<COUNT;I++)
    {
        SBUF=*BASE_ADDR;
        while(TI!=1);
        TI=0;
    
        BASE_ADDR++;
          
    }
        
    SBUF=EOM;
    while(TI!=1);
    TI=0;
    
}

 void RX_REPLY_2_SEND_MSG()
{    
    unsigned char RX;
    
    RX=0;
    while(RX!='>')
    {
        while(RI!=1);
        RX=SBUF;
        RI=0;
            
    }
}