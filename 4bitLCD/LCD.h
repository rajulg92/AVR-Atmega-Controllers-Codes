/*	LCD.h
*
*	This header files have implementation of some common finctions of the LCD.
*	Include this header file in your C code. Call the lcd_init() function before
	calling any other functions to initialize the lcd.


	lcd_init() ----- Call the lcd_init() function before calling any other functions
			  to initialize the lcd.
	lcd_cmd() ------ This function is used to give any command instructions to the
			  LCD. For e.g lcd_cmd(0x01) will give the clear command.
	lcd_char() ----- This function will display a single character on the LCD display.
			  For example lcd_char(0x61) will display A. Again lcd_char('b')
			  will display b.
	lcd_string()---- This function will display a string. An example of this would be
			  like lcd_string("This is AVR")
	lcd_showvalue()- This will show a 3-digit decimal value on the LCD. For example if
			  we give lcd_showvalue(0xFF) then 255 will be displayed.
	lcd_gotoxy1()--- Will set the cursor at a particular position on LINE1 of the LCD.
			  So lcd_gotoxy1(3) will set the cursor at the 3+1=4th column in
			  the 1st line of the LCD.
	lcd_gotoxy2()--- Will set the cursor at a particular position on LINE1 of the LCD.
			  So lcd_gotoxy2(0) will set the cursor at the 0+1=1st column in
			  the 2nd line of the LCD.

	lcd_exit() ----- You may call this function after you are over with your LCD.While
			  calling the lcd functions, there would be some changes to the
			  PORTC. This restores the original PORTC configuration before
			  calling the lcd_init() function. This is not absolutely necessary.
			  You may or may not use this function.
*
*
*
*/





#define LINE1	0x80
#include<util/delay.h>
#define LINE2	0xC0
unsigned char k=0,j=0;
unsigned char save=0;

unsigned char ch[10] = {'0','1','2','3','4','5','6','7','8','9'};
unsigned char tenthousand=0,thousand=0,hundred=0,ten=0,one=0;

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void toggleE(void);
void dely_ns(unsigned char tim);
void lcd_char(unsigned char single);
void lcd_string(unsigned char str[32]);
void lcd_showvalue(unsigned char num);
void lcd_gotoxy1(unsigned char pos);
void lcd_gotoxy2(unsigned char pos);
void lcd_exit(void);
void WaitMs(unsigned int ms);

void lcd_init(void)	//init the lcd
{	
	DDRA=0xF0;
	DDRC=0x80;
	DDRG=0x04;
	WaitMs(15);
	CLEARBIT(PORTB,BIT(5)+BIT(6)+BIT(7));
	PORTA=0x00;
	lcd_cmd(0x20);		//data is 8-bit length
	WaitMs(1);
	lcd_cmd(0x20);		//data is 8-bit length
	WaitMs(1);
	lcd_cmd(0x20);		//data is 8-bit length
	WaitMs(1);
	lcd_cmd(0x28);		//sets character font
	WaitMs(1);
	lcd_cmd(0x0E);		//display off(d,c,b=0) 08
	WaitMs(1);
	lcd_cmd(0x01);		//clears the data
	WaitMs(1);
	//lcd_cmd(0x02);		//home
	//WaitMs(1);
	lcd_cmd(0x6);		//entry mode--increment
	WaitMs(1);
	//lcd_cmd(0x0C);		//display--on,cursor--on.blinking--on
	//WaitMs(1);

	
}

void toggleE(void)
{
SETBIT(PORTC,BIT(7));
_delay_us(500);
CLEARBIT(PORTC,BIT(7));
}

void dely_ns(unsigned char tim)
{
	for(j=0;j<tim;j++)
	{
		for(k=0;k<10;k++)
		{};
	}
}

void lcd_cmd(unsigned char ch)
{
	CLEARBIT(PORTG,BIT(2));
	CLEARBIT(PORTC,BIT(6)+BIT(7));
	PORTA = (ch & 0xF0);			// Send twice because ATMEGA is too fast 
  	PORTA = (ch & 0xF0); 			// for the LCD display !
  	PORTA = ch & 0xF0;
   	PORTA = ch & 0xF0;
	toggleE();
   // Extract lower nibble
   //
   PORTA = ((ch << 4) & 0xF0); 	// Send twice because ATMEGA is too fast 
   PORTA = ((ch << 4) & 0xF0);	// for the LCD display !
   PORTA = (ch << 4) & 0xF0;
   PORTA = (ch << 4) & 0xF0;
	toggleE();
   PORTA = 0x00;
   WaitMs(10);


}

void lcd_num1(unsigned int value)
{
tenthousand=value/10000;
value=value%10000;
thousand=value/1000;
value=value%1000;
hundred=value/100;
value=value%100;
ten=value/10;
one=value%10;
//ten=(value - (hundred*100))/10;
//one=(value - (hundred*100) - (ten*10));

lcd_char(ch[tenthousand]);
lcd_char(ch[thousand]);
lcd_char(ch[hundred]);
lcd_char(ch[ten]);
lcd_char(ch[one]);
lcd_char(' ');

/*if(lcd_count==3)
	{
	lcd_count=0;
	lcd_cmd(PUTLINE1);
	line_count++;
	if(line_count%2==0)
		{
		
		lcd_cmd(PUTLINE2);
		}
	else
		{
		lcd_cmd(PUTLINE1);
		}
	}*/
}

void lcd_char(unsigned char data)
{	WaitMs(15);
	SETBIT(PORTG,BIT(2));
	CLEARBIT(PORTC,BIT(6)+BIT(7));
	 PORTA = (data & 0xF0);
	PORTA = (data & 0xF0);
	PORTA = (data & 0xF0);
	 toggleE();
	CLEARBIT(PORTG,BIT(2));
	CLEARBIT(PORTC,BIT(5)+BIT(6)+BIT(7));
	SETBIT(PORTG,BIT(2));
	PORTA = (data << 4);
	PORTA = (data << 4);
	PORTA = (data << 4);						
	toggleE();
}

void lcd_string(unsigned char str[32])
{
	unsigned char k=0;
	PORTA=0x01;
	SETBIT(PORTG,BIT(2));
	CLEARBIT(PORTC,BIT(6)+BIT(7));
	while(str[k]!='\0')	//Till null character is encountered
		{
			if(k==16)
				lcd_cmd(LINE2);
			PORTA=((str[k])&0xF0)|0x01;
			toggleE();
			PORTA=((str[k]&(0x0F))*16)|0x01;
			toggleE();
			k++;
			WaitMs(1);
		}
}


void lcd_showvalue(unsigned char num)	//prints the decimal 3digit value of num
{
	unsigned char H=0,T=0,O=0;
	H=num/100;
	T=(num - (H*100))/10;
	O=(num - (H*100) - (T*10));
	
	lcd_char(H+48);
	lcd_char(T+48);
	lcd_char(O+48);
}

void lcd_gotoxy1(unsigned char pos)
{
	lcd_cmd(LINE1+pos);
}

void lcd_gotoxy2(unsigned char pos)
{
	lcd_cmd(LINE2+pos);
}
		

void lcd_exit(void)
{
	PORTA=save;
}

/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void WaitMs(unsigned int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 16380 (16k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 3276; ++i)
			asm("nop");
	}
}
