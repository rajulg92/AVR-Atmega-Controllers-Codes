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

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b		//Toggles the particular bit

unsigned int compute_duty_cycle(unsigned int dc,char ix);

typedef enum DirectionBIT
{
	CLEAR = 0,SET = 1, TOGGLE = 2
}DirectionBIT;


void PWM(char SELECT1 , char SELECT2 ,unsigned int f , unsigned int dc,enum DirectionBIT bit)
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
		duty_cycle = compute_duty_cycle(dc,ix);

		TCCR1A = 0xA8;
		TCCR1B = 0x11;

		if(cy == 'A')
		  {
		   SETBIT(DDRB,BIT(5));
		   OCR1A = duty_cycle;
		   SETBIT(DDRL,BIT(2));		   
		   if (bit == CLEAR)
		   {
			   CLEARBIT(PORTL,BIT(2));
		   }
		   else if (bit == SET)
		   {
			   SETBIT(PORTL,BIT(2));
		   }
		   else if (bit == TOGGLE)
		   {
			   TOGGLEBIT(PORTL,BIT(2));
		   }
		  }		
		else if(cy == 'B')
		  {
		   SETBIT(DDRB,BIT(6));
		   OCR1B = duty_cycle;
		   SETBIT(DDRL,BIT(6));
		   if (bit == CLEAR)
		   {
			   CLEARBIT(PORTL,BIT(6));
		   }
		   else if (bit == SET)
		   {
			   SETBIT(PORTL,BIT(6));
		   }
		   else if (bit == TOGGLE)
		   {
			   TOGGLEBIT(PORTL,BIT(6));
		   }
		  }
		else
		  {
		   SETBIT(DDRB,BIT(7));
		   OCR1C = duty_cycle;
		   
		   		   SETBIT(DDRL,BIT(7));
		   		   if (bit == CLEAR)
		   		   {
			   		   CLEARBIT(PORTL,BIT(7));
		   		   }
		   		   else if (bit == SET)
		   		   {
			   		   SETBIT(PORTL,BIT(7));
		   		   }
		   		   else if (bit == TOGGLE)
		   		   {
			   		   TOGGLEBIT(PORTL,BIT(7));
		   		   }
		  }	
	}

	if(ix == '3')
	{
		TCCR3A = 0xA8;
		TCCR3B = 0x11;
		
		ICR3 = frequency;
		duty_cycle = compute_duty_cycle(dc,ix);
		
 		
		if(cy == 'A')
		  { 
			   SETBIT(DDRE,BIT(3));
			   OCR3A = duty_cycle;
			   
			   	SETBIT(DDRH,BIT(6));
			   	if (bit == CLEAR)
			   	{
				   	CLEARBIT(PORTH,BIT(6));
			   	}
			   	else if (bit == SET)
			   	{
				   	SETBIT(PORTH,BIT(6));
			   	}
			   	else if (bit == TOGGLE)
			   	{
				   	TOGGLEBIT(PORTH,BIT(6));
			   	}
		  }		
		else if(cy == 'B')
		  {
			   SETBIT(DDRE,BIT(4));
			   OCR3B = duty_cycle;
			   
			   	SETBIT(DDRL,BIT(7));
			   	if (bit == CLEAR)
			   	{
				   	CLEARBIT(PORTH,BIT(7));
			   	}
			   	else if (bit == SET)
			   	{
				   	SETBIT(PORTH,BIT(7));
			   	}
			   	else if (bit == TOGGLE)
			   	{
				   	TOGGLEBIT(PORTH,BIT(7));
			   	}
		  }
		else
		  {
			   SETBIT(DDRE,BIT(5));
			   OCR3C = duty_cycle;
			   
		   		SETBIT(DDRG,BIT(3));
		   		if (bit == CLEAR)
		   		{
			   		CLEARBIT(PORTG,BIT(3));
		   		}
		   		else if (bit == SET)
		   		{
			   		SETBIT(PORTG,BIT(3));
		   		}
		   		else if (bit == TOGGLE)
		   		{
			   		TOGGLEBIT(PORTG,BIT(3));
		   		}			   			   
		  }			  
	}

	if(ix == '4')
	{	
		ICR4 = frequency;
		duty_cycle = compute_duty_cycle(dc,ix);		
 		TCCR4A = 0xA8;
		TCCR4B = 0x11;
		if(cy == 'A')
		  { 
			SETBIT(DDRH,BIT(3));
			OCR4A = duty_cycle;
			
				SETBIT(DDRG,BIT(4));
				if (bit == CLEAR)
				{
					CLEARBIT(PORTG,BIT(4));
				}
				else if (bit == SET)
				{
					SETBIT(PORTG,BIT(4));
				}
				else if (bit == TOGGLE)
				{
					TOGGLEBIT(PORTG,BIT(4));
				}
		  }		
		else if(cy == 'B')
		  {
			SETBIT(DDRH,BIT(4));
		   OCR4B = duty_cycle;
		   
		   		SETBIT(DDRL,BIT(0));
		   		if (bit == CLEAR)
		   		{
			   		CLEARBIT(PORTL,BIT(0));
		   		}
		   		else if (bit == SET)
		   		{
			   		SETBIT(PORTL,BIT(0));
		   		}
		   		else if (bit == TOGGLE)
		   		{
			   		TOGGLEBIT(PORTL,BIT(0));
		   		}
		  }
		else
		  {
			  SETBIT(DDRH,BIT(5));
		      OCR4C = duty_cycle;
			  
			  	SETBIT(DDRL,BIT(1));
			  	if (bit == CLEAR)
			  	{
				  	CLEARBIT(PORTL,BIT(1));
			  	}
			  	else if (bit == SET)
			  	{
				  	SETBIT(PORTL,BIT(1));
			  	}
			  	else if (bit == TOGGLE)
			  	{
				  	TOGGLEBIT(PORTL,BIT(1));
			  	}
		  }		  
	}

	if(ix == '5')
	{
		ICR5 = frequency;
		duty_cycle = compute_duty_cycle(dc,ix);
 		TCCR5A = 0xA8;
		TCCR5B = 0x11;
		if(cy == 'A')
		  { 
			SETBIT(DDRL,BIT(3));
		   OCR5A = duty_cycle;
		   
		   	SETBIT(DDRG,BIT(0));
		   	if (bit == CLEAR)
		   	{
			   	CLEARBIT(PORTG,BIT(0));
		   	}
		   	else if (bit == SET)
		   	{
			   	SETBIT(PORTG,BIT(0));
		   	}
		   	else if (bit == TOGGLE)
		   	{
			   	TOGGLEBIT(PORTG,BIT(0));
		   	}
		  }		
		else if(cy == 'B')
		  {
			  SETBIT(DDRL,BIT(4));
		   OCR5B = duty_cycle;
		   
		   			  	SETBIT(DDRG,BIT(1));
		   			  	if (bit == CLEAR)
		   			  	{
			   			  	CLEARBIT(PORTG,BIT(1));
		   			  	}
		   			  	else if (bit == SET)
		   			  	{
			   			  	SETBIT(PORTG,BIT(1));
		   			  	}
		   			  	else if (bit == TOGGLE)
		   			  	{
			   			  	TOGGLEBIT(PORTG,BIT(1));
		   			  	}
		  }
		else
		  {
			  SETBIT(DDRL,BIT(5));
			  OCR5C = duty_cycle;
			  
			  	SETBIT(DDRG,BIT(2));
			  	if (bit == CLEAR)
			  	{
				  	CLEARBIT(PORTG,BIT(2));
			  	}
			  	else if (bit == SET)
			  	{
				  	SETBIT(PORTG,BIT(2));
			  	}
			  	else if (bit == TOGGLE)
			  	{
				  	TOGGLEBIT(PORTG,BIT(2));
			  	}
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
