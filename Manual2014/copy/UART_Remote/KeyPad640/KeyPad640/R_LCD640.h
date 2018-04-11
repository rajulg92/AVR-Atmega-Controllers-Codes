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

#define BIT(x)  (1<<(x))
#define CHECKBIT(x,b)  x&b
#define SETBIT(x,b)  x|=b;
#define CLEARBIT(x,b)  x&=~b;
#define TOGGLEBIT(x,b)  x^=b;
#define LINE1	0x80
#include<util/delay.h>
#define LINE2	0xC0
#include <math.h>
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
void lcd_showvalue(unsigned long int num);
void lcd_showvaluef(float);
void lcd_gotoxy1(unsigned char pos);
void lcd_gotoxy2(unsigned char pos);
void lcd_exit(void);
void WaitMs(unsigned int ms);

void lcd_init(void)	//init the lcd
{	
	DDRE=0xC7;
	//DDRC=0x80;
	DDRG=0x20;
	WaitMs(15);
	CLEARBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
	PORTE=0x00;
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
SETBIT(PORTE,BIT(0));
_delay_us(500);
CLEARBIT(PORTE,BIT(0));
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
	int temp;
	CLEARBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
					/*PORTA = (ch & 0xF0);			// Send twice because ATMEGA is too fast 
  					PORTA = (ch & 0xF0); 			// for the LCD display !
  					PORTA = ch & 0xF0;
   					PORTA = ch & 0xF0;*/

	temp= ch & 0b11000000; // '10'000000
	PORTE=temp;
	temp=(ch >> 3) & 0b00000110; // 00000'11'0
	PORTE= PORTE | temp;
	_delay_us(5);
	toggleE();
   // Extract lower nibble
	SETBIT(PORTG,BIT(5));
   	CLEARBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
   					/*PORTA = ((ch << 4) & 0xF0); 	// Send twice because ATMEGA is too fast 
   					PORTA = ((ch << 4) & 0xF0);	// for the LCD display !
   					PORTA = (ch << 4) & 0xF0;
   					PORTA = (ch << 4) & 0xF0;*/
	temp=(ch << 4) & 0b11000000; // '10'000000
	PORTE=temp;
	temp=(ch << 1) & 0b00000110; // 00000'01'0 
	PORTE= PORTE | temp;
	_delay_us(5);
	toggleE();
   	PORTE = 0x00;
   	WaitMs(10);


}


void lcd_char(unsigned char data)
{	
	int temp;
	WaitMs(15);
	SETBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
	temp= (data & 0b11000000); // '10'000000
	PORTE=temp;
	temp=(data>>3) & 0b00000110; // 00000'11'0
	PORTE= PORTE | temp;
	_delay_us(5);
	toggleE();				// '10'000'11'0 (first nibble Done)
							/* PORTA = (data & 0xF0);
							PORTA = (data & 0xF0);
							PORTA = (data & 0xF0);*/

	CLEARBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
	SETBIT(PORTG,BIT(5));
	temp=(data << 4) & 0b11000000; // '10'000000
	PORTE=temp;
	temp=(data << 1) & 0b00000110; // 00000'01'0 
	PORTE= PORTE | temp;
	_delay_us(5);
						/*PORTA = (data << 4);
						PORTA = (data << 4);
						PORTA = (data << 4);	*/					
	toggleE();
}

void lcd_string(unsigned char str[32])
{
	int poop,bitch;
	unsigned char k=0;
	PORTA=0x01;
	SETBIT(PORTG,BIT(5));
	CLEARBIT(PORTE,BIT(0));
	while(str[k]!='\0')	//Till null character is encountered
		{
			if(k==16)
				lcd_cmd(LINE2);
			SETBIT(PORTG,BIT(5));
			CLEARBIT(PORTE,BIT(0));
			bitch=str[k]; //10111001
			poop= bitch & 0b11000000; // '10'000000
			PORTE=poop;
			poop=(bitch>>3) & 0b00000110; // 00000'11'0
			PORTE= PORTE | poop;
			//PORTA=((str[k])&0xF0)|0x01;
			toggleE();
			CLEARBIT(PORTG,BIT(5))
			SETBIT(PORTG,BIT(5));
			CLEARBIT(PORTE,BIT(0));
			bitch=str[k];
			poop=(bitch << 4) & 0b11000000; // '10'000000
			PORTE=poop;
			poop=(bitch << 1) & 0b00000110; // 00000'01'0 
			PORTE= PORTE | poop;
			//PORTA=((str[k]&(0x0F))*16)|0x01;
			toggleE();
			k++;
			WaitMs(1);
		}
}


void lcd_showvalue(unsigned long int num)
{
	unsigned char H=0;
	
	unsigned long int n = 0;
	int flag = 0;


	if(num%100000 == 0)
	{
		flag = 5;
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



	while(num!=0)
	{
	
			H = num%10;
			num = num/10;
			n = (n*10)+H;

	}
	
	while(n!=0)
	{
	
			H = n%10;
			n = n/10;
			lcd_char(H+48);

	}
	
	
	while(flag > 0)
	{
		lcd_char(48);
		flag--;
	}	

}


void lcd_showvaluef(float num)
{

	unsigned long int l;
	double fraction,integer;

	l = (unsigned long int)num;

	lcd_showvalue(l);

	fraction = modf((double)num,&integer);

	l = (unsigned long int)(fraction * 100000);

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
