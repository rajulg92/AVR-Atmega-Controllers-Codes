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
			  PORTB. This restores the original PORTB configuration before
			  calling the lcd_init() function. This is not absolutely necessary.
			  You may or may not use this function.
*
*
*pd2 = rs
pd4 e
pa data
*/

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b		//Toggles the particular bit
#define Enable  BIT(3)
#define RS	BIT(4)
#define RW  BIT(5)

#define LINE1	0x80
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
void lcd_showvalue(long int num);
void lcd_gotoxy1(unsigned char pos);
void lcd_gotoxy2(unsigned char pos);
void lcd_exit(void);
void WaitMs(unsigned int ms);

void lcd_init(void)	//init the lcd
{	
	DDRC=0xFF;				//lcd data bits
	DDRD|= (1<<PD3) |(1<<PD4) |(1<<PD5);				
	toggleE();
	SETBIT(PORTD, RS);		// RS
	lcd_cmd(0x38);		// Initialization in 8bit mode
	lcd_cmd(0x01);		// Clear LCD
	lcd_cmd(0x0E);		// Cursor ON
	lcd_cmd(LINE1);	
}

void toggleE(void)
{
SETBIT(PORTD, Enable);
WaitMs(1);
CLEARBIT(PORTD, Enable);
}

void delay_ns(unsigned char tim)
{
	for(j=0;j<tim;j++)
	{
		for(k=0;k<10;k++)
		{};
	}
}

void lcd_cmd(unsigned char cmd)
{   
    WaitMs(2);
	CLEARBIT(PORTD, RS);		// RS
	PORTC=cmd;
	//SETBIT(PORTD,BIT(3));	// ENABLE PD0 = 1
		// RS PD1 = 0
	//CLEARBIT(PORTD,BIT(5));	// RW PD2 = 0	
	toggleE();
}

void lcd_char(unsigned char single)
{
    WaitMs(1);
		
	SETBIT(PORTD, Enable);		//Enable
	SETBIT(PORTD, RS);
	PORTC=single;		//RS
	//CLEARBIT(PORTD,BIT(3));		//RW
	toggleE();

}

void lcd_string(unsigned char str[32])
{
	unsigned char k=0;
	SETBIT(PORTD, RS);
	while(str[k]!='\0')	//Till null character is encountered
		{
			WaitMs(1);
			if(k==16)
				lcd_cmd(LINE2);
			PORTC=str[k];
			toggleE();
			k++;
		}
}


void lcd_showvalue(long int num)	//prints the decimal 3digit value of num
{
	unsigned char H=0;
	int n = 0,flag = 0;

	if(num%10000 == 0)
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
		lcd_char(H+48);

	}
	
	
	while(flag > 0)
	{
		lcd_char('0');
		flag--;
	}

}

void lcd_showvaluef(float num)
{
	unsigned long l;

	l = (int)num;

	lcd_showvalue(l);

	num = num - l;

	num = num * 10000;

	l = (int)num;

	lcd_char('.');

	lcd_showvalue(l);
}


void lcd_gotoxy1(unsigned char pos)
{
	lcd_cmd(LINE1+pos);
}


void lcd_gotoxy2(unsigned char pos)
{
	lcd_cmd(LINE2+pos);
}
		

/*void lcd_exit(void)
{
	PORTB=save;
}*/

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