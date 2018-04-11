/*
NAME OF FUNCTION - PWM()
PURPOSE	- PWM Generation AVR 640
RETURN TYPE - VOID
PARAMETERS - 
	SELECT1 : NUMBER	eg. TCCR1y , TCCR3y
	SELECT2 : CHAR		eg. TCCRxA , TCCRxB
	VTCCR , VICR , VOCR :   Value of TCCR, ICR , OCR registers respectively.
*/
#include<ctype.h>

#define FCLK 14745600UL

#define check(x,b) x&b
#define clear(x,b) x&=~b
#define set(x,b) x|=b
#define bit(x) (1<<(x))
#define toggle(x,b) x^=b

unsigned int compute_duty_cycle(unsigned int dc,char ix);
void setPWM(char SELECT1 , char SELECT2 ,unsigned int f)
{
	char ix;
	char cy;
	unsigned int  frequency,duty_cycle;

	ix = toupper(SELECT1);
	cy = toupper(SELECT2);

	frequency = (FCLK / (2*f) );	//1 is selected as presclar

	if(ix == '1')
	{
		ICR1 = frequency;

		TCCR1A = 0xA8;
		TCCR1B = 0x11;
	}

	if(ix == '3')
	{
		
		TCCR3A = 0xA8;
		TCCR3B = 0x11;
		
		ICR3 = frequency;
	}

	if(ix == '4')
	{	
		ICR4 = frequency;

 		TCCR4A = 0xA8;
		TCCR4B = 0x11;

	}

	if(ix == '5')
	{
		
		ICR5 = frequency;
 		TCCR5A = 0xA8;
		TCCR5B = 0x11;
	}
}


void PWM(char SELECT1 , char SELECT2 ,unsigned int f , unsigned int dc)
{
	char ix;
	char cy;
	unsigned int  frequency,duty_cycle;

	ix = toupper(SELECT1);
	cy = toupper(SELECT2);

	if(ix == '1')
	{

		duty_cycle = compute_duty_cycle(dc,ix);

		if(cy == 'A')
		  { 		   
		   set(DDRB,bit(5));
		   OCR1A = duty_cycle;
		  }		
		else if(cy == 'B')
		  {		   
		   set(DDRB,bit(6));		   
		   OCR1B = duty_cycle;
		  }

		else
		  {
		  	set(DDRB,bit(7));
		   OCR1C = duty_cycle;
		  }	
	}

	if(ix == '3')
	{
		
		duty_cycle = compute_duty_cycle(dc,ix);
		
		if(cy == 'A')
		  { 
		   set(DDRE,bit(3));
		   OCR3A = duty_cycle;
		  }		
		else if(cy == 'B')
		  {
		   set(DDRE,bit(4));
		   OCR3B = duty_cycle;
		  }
		else
		  {
		   set(DDRE,bit(5));
		   OCR3C = duty_cycle;
		  }			  
	}

	if(ix == '4')
	{
		
	
		duty_cycle = compute_duty_cycle(dc,ix);		
 
 		if(cy == 'A')
		  { 
		   set(DDRH,bit(3));
		   OCR4A = duty_cycle;
		  }		
		else if(cy == 'B')
		  {
		   set(DDRH,bit(4));		   
		   OCR4B = duty_cycle;
		  }
		else
		  {
		   set(DDRH,bit(5));
		   OCR4C = duty_cycle;
		  }		  
	}

	if(ix == '5')
	{	
		duty_cycle = compute_duty_cycle(dc,ix);

		if(cy == 'A')
		  { 
		   set(DDRL,bit(3));
		   OCR5A = duty_cycle;
		  }		
		else if(cy == 'B')
		  {
		   set(DDRL,bit(4));
		   OCR5B = duty_cycle;
		  }
		else
		  {
			set(DDRL,bit(5));
		   OCR5C = duty_cycle;
		  }	
	}

}



unsigned int compute_duty_cycle(unsigned int dc,char ix)
{
	
if (ix == '1')
	return 	( ((ICR1 /100 ) * dc));
if (ix == '3')
	return 	( ((ICR3 /100 ) * dc));
if(ix == '4')
	return 	( ((ICR4 /100 ) * dc));
if(ix == '5')
	return 	( ((ICR5 /100 ) * dc));
}
