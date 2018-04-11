/******************************************************************
*  Super amazing PS2 controller Arduino Library v1.8
*		details and example sketch: 
*			http://www.billporter.info/?p=240
*
*    Original code by Shutter on Arduino Forums
*
*    Revamped, made into lib by and supporting continued development:
*              Bill Porter
*              www.billporter.info
*
*	 Contributers:
*		Eric Wetzel (thewetzel@gmail.com)
*		Kurt Eckhardt
*
*  Lib version history
*    0.1 made into library, added analog stick support. 
*    0.2 fixed config_gamepad miss-spelling
*        added new functions:
*          NewButtonState();
*          NewButtonState(unsigned int);
*          ButtonPressed(unsigned int);
*          ButtonReleased(unsigned int);
*        removed 'PS' from begining of ever function
*    1.0 found and fixed bug that wasn't configuring controller
*        added ability to define pins
*        added time checking to reconfigure controller if not polled enough
*        Analog sticks and pressures all through 'ps2x.Analog()' function
*        added:
*          enableRumble();
*          enablePressures();
*    1.1  
*        added some debug stuff for end user. Reports if no controller found
*        added auto-increasing sentence delay to see if it helps compatibility.
*    1.2
*        found bad math by Shutter for original clock. Was running at 50kHz, not the required 500kHz. 
*        fixed some of the debug reporting. 
*	1.3 
*	    Changed clock back to 50kHz. CuriousInventor says it's suppose to be 500kHz, but doesn't seem to work for everybody. 
*	1.4
*		Removed redundant functions.
*		Fixed mode check to include two other possible modes the controller could be in.
*       Added debug code enabled by compiler directives. See below to enable debug mode.
*		Added button definitions for shapes as well as colors.
*	1.41
*		Some simple bug fixes
*		Added Keywords.txt file
*	1.5
*		Added proper Guitar Hero compatibility
*		Fixed issue with DEBUG mode, had to send serial at once instead of in bits
*	1.6
*		Changed config_gamepad() call to include rumble and pressures options
*			This was to fix controllers that will only go into config mode once
*			Old methods should still work for backwards compatibility 
*    1.7
*		Integrated Kurt's fixes for the interrupts messing with servo signals
*		Reorganized directory so examples show up in Arduino IDE menu
*    1.8
*		Added Arduino 1.0 compatibility. 
*
*
*
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*  
******************************************************************/


/*
ChangeLogs
1. Functions having return type as boolean are changed to bool 
2. Functions where uint8_t was used as a return type or a parameter is changed to uint8_t
3. The Function NewButtonState cannot be called with a void type. in order to call it with a void type call it with 0xFFFF
4. In order to call read_gamepad(void) call it as read_gamepad(false, 0x00)
5. In order to call function return config_gamepad(clk, cmd, att, dat) without last two parameters call it as return config_gamepad(clk, cmd, att, dat, false, false)
*/



// $$$$$$$$$$$$ DEBUG ENABLE SECTION $$$$$$$$$$$$$$$$
// to debug ps2 controller, uncomment these two lines to print out debug to uart

//#define PS2X_DEBUG
//#define PS2X_COM_DEBUG


#ifndef PS2X_lib_h
#define PS2X_lib_h


/*#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif*/
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define CTRL_CLK        4
#define CTRL_uint8_t_DELAY 3

//These are our button constants
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_GREEN       0x1000
#define PSB_RED         0x2000
#define PSB_BLUE        0x4000
#define PSB_PINK        0x8000
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000

//Guitar  button constants
#define GREEN_FRET		0x0200
#define RED_FRET		0x2000
#define YELLOW_FRET		0x1000
#define BLUE_FRET		0x4000
#define ORANGE_FRET		0x8000
#define STAR_POWER		0x0100
#define UP_STRUM		0x0010
#define DOWN_STRUM		0x0040
#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

//These are analog buttons
#define PSAB_PAD_RIGHT   9
#define PSAB_PAD_UP      11
#define PSAB_PAD_DOWN    12
#define PSAB_PAD_LEFT    10
#define PSAB_L2          19
#define PSAB_R2          20
#define PSAB_L1          17
#define PSAB_R1          18
#define PSAB_GREEN       13
#define PSAB_RED         14
#define PSAB_BLUE        15
#define PSAB_PINK        16
#define PSAB_TRIANGLE    13
#define PSAB_CIRCLE      14
#define PSAB_CROSS       15
#define PSAB_SQUARE      16


#define SET(x,y) (x|=(1<<y))
#define CLR(x,y) (x&=(~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))



#define true 1
#define false 0



/***************************************************************************************/
static uint8_t enter_config[]={0x01,0x43,0x00,0x01,0x00};
static uint8_t set_mode[]={0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};
static uint8_t set_uint8_ts_large[]={0x01,0x4F,0x00,0xFF,0xFF,0x03,0x00,0x00,0x00};
static uint8_t exit_config[]={0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static uint8_t enable_rumble[]={0x01,0x4D,0x00,0x00,0x01};
static uint8_t type_read[]={0x01,0x45,0x00,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A};

unsigned char PS2data[21];
unsigned char i;
unsigned int last_buttons;
unsigned int buttons;
uint8_t _clk_mask; 
volatile uint8_t *_clk_oreg;
uint8_t _cmd_mask; 
volatile uint8_t *_cmd_oreg;
uint8_t _att_mask; 
volatile uint8_t *_att_oreg;
uint8_t _dat_mask; 
volatile uint8_t *_dat_ireg;
unsigned long last_read;
uint8_t read_delay;
uint8_t controller_type;
uint8_t en_Rumble;
uint8_t en_Pressures;

/*
uint8_t NewButtonState(void) {
   return ((last_buttons ^ buttons) > 0);

}
*/
  
uint8_t Button(uint16_t button) {
   return ((~buttons & button) > 0);
}

uint8_t NewButtonState(unsigned int button) {
  return (((last_buttons ^ buttons) & button) > 0);
}

uint8_t ButtonPressed(unsigned int button) {
  return(NewButtonState(button) & Button(button));
}

uint8_t ButtonReleased(unsigned int button) {
  return((NewButtonState(button)) & ((~last_buttons & button) > 0));
}


unsigned int ButtonDatauint8_t(void) {
   return (~buttons);
}

uint8_t Analog(uint8_t button) {
  return PS2data[button];
}
unsigned char _gamepad_shiftinout (char uint8_t) {
  // uint8_t old_sreg = SREG;        // *** KJE *** save away the current state of interrupts
  

   unsigned char tmp = 0;
   //cli();                          // *** KJE *** disable for now
   for(i=0;i<8;i++) {

	  if(CHK(uint8_t,i)) SET(*_cmd_oreg,_cmd_mask);
	  else  CLR(*_cmd_oreg,_cmd_mask);
	  CLR(*_clk_oreg,_clk_mask);

     // SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	  _delay_us(CTRL_CLK);
	  //cli();	// *** KJE ***

	  if(CHK(*_dat_ireg,_dat_mask)) SET(tmp,i);
	  SET(*_clk_oreg,_clk_mask);
   }
   SET(*_cmd_oreg,_cmd_mask);
   //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
   _delay_us(CTRL_uint8_t_DELAY);
   return tmp;
}
/*
void read_gamepad(void) {
    read_gamepad(false, 0x00);
}
*/

void read_gamepad(uint8_t motor1, uint8_t motor2) {
  double temp = millis() - last_read;
 // uint8_t old_sreg = SREG;        // *** KJE **** save away the current state of interrupts - *** *** KJE *** ***
  
  if (temp > 1500) //waited to long
    reconfig_gamepad();
    
  if(temp < read_delay)  //waited too short
    _delay_ms(read_delay - temp);
    
    
  last_buttons = buttons; //store the previous buttons states

  if(motor2 != 0x00)
    motor2 = map(motor2,0,255,0x40,0xFF); //noting below 40 will make it spin
  
 // cli();	//*** KJE ***  
  SET(*_cmd_oreg,_cmd_mask);
  SET(*_clk_oreg,_clk_mask);
  CLR(*_att_oreg,_att_mask); // low enable joystick
  //SREG = old_sreg;  // *** KJE *** - Interrupts may be enabled again
  
  _delay_ms(CTRL_uint8_t_DELAY);
  //Send the command to send button and joystick data;
  char dword[9] = {0x01,0x42,0,motor1,motor2,0,0,0,0};
  uint8_t dword2[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

  for (int i = 0; i<9; i++) {
	  PS2data[i] = _gamepad_shiftinout(dword[i]);
  }
  if(PS2data[1] == 0x79) {  //if controller is in full data return mode, get the rest of data
       for (int i = 0; i<12; i++) {
			PS2data[i+9] = _gamepad_shiftinout(dword2[i]);
       }
  }
    
  //cli();
  SET(*_att_oreg,_att_mask); // HI disable joystick
  //SREG = old_sreg;  // Interrupts may be enabled again    
	
	#ifdef PS2X_COM_DEBUG
    /*Serial.println("OUT:IN");
		for(int i=0; i<9; i++){
			Serial.print(dword[i], HEX);
			Serial.print(":");
			Serial.print(PS2data[i], HEX);
			Serial.print(" ");
		}
		for (int i = 0; i<12; i++) {
			Serial.print(dword2[i], HEX);
			Serial.print(":");
			Serial.print(PS2data[i+9], HEX);
			Serial.print(" ");
		}
	Serial.println("");*/	
	#endif
	
   buttons = *(uint16_t*)(PS2data+3);   //store as one value for multiple functions
   last_read = millis();
}

void sendCommandString(uint8_t string[], uint8_t len) {
  
   //uint8_t old_sreg = SREG;        // *** KJE *** save away the current state of interrupts

  #ifdef PS2X_COM_DEBUG
  uint8_t temp[len];
  //cli();                          // *** KJE *** disable for now
  CLR(*_att_oreg,_att_mask); // low enable joystick
  //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	
  for (int y=0; y < len; y++)
    temp[y] = _gamepad_shiftinout(string[y]);
    
  //cli();                          // *** KJE *** disable for now
  SET(*_att_oreg,_att_mask); //high disable joystick  
  //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
   _delay_ms(read_delay);                  //wait a few
  
  //Serial.println("OUT:IN Configure");
/*  for(int i=0; i<len; i++){
			Serial.print(string[i], HEX);
			Serial.print(":");
			Serial.print(temp[i], HEX);
			Serial.print(" ");
		}
   Serial.println("");
 */ 
  #else
  //cli();                          // *** KJE *** disable for now
  CLR(*_att_oreg,_att_mask); // low enable joystick
  //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
  for (int y=0; y < len; y++)
    _gamepad_shiftinout(string[y]);
    
   //cli();                          // *** KJE *** disable for now
   SET(*_att_oreg,_att_mask); //high disable joystick  
   //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
   _delay_ms(read_delay);                  //wait a few
   #endif
}



/*
uint8_t config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat) {
	return config_gamepad(clk, cmd, att, dat, false, false);
}
*/

uint8_t config_gamepad(uint8_t clk, uint8_t cmd, uint8_t att, uint8_t dat, uint8_t pressures, uint8_t rumble) {

   //uint8_t old_sreg = SREG;        // *** KJE *** save away the current state of interrupts
   uint8_t temp[sizeof(type_read)];
  
 _clk_mask = maskToBitNum(digitalPinToBitMask(clk));
 _clk_oreg = portOutputRegister(digitalPinToPort(clk));
 _cmd_mask = maskToBitNum(digitalPinToBitMask(cmd));
 _cmd_oreg = portOutputRegister(digitalPinToPort(cmd));
 _att_mask = maskToBitNum(digitalPinToBitMask(att));
 _att_oreg = portOutputRegister(digitalPinToPort(att));
 _dat_mask = maskToBitNum(digitalPinToBitMask(dat));
 _dat_ireg = portInputRegister(digitalPinToPort(dat));
  

  /*pinMode(clk, OUTPUT); //configure ports
  pinMode(att, OUTPUT);
  pinMode(cmd, OUTPUT);
  pinMode(dat, INPUT);

  digitalWrite(dat, HIGH); //enable pull-up 
  */
  DDRD |= (1 << clk ) | (1 << att) | (1 << cmd);
  DDRD &= ~(1 << dat);
  PORTD |= (1 << dat);


  
   //cli();                          // *** KJE *** disable for now
   SET(*_cmd_oreg,_cmd_mask); // SET(*_cmd_oreg,_cmd_mask);
   SET(*_clk_oreg,_clk_mask);
   //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
   
   //new error checking. First, read gamepad a few times to see if it's talking
   read_gamepad(false, 0x00);
   read_gamepad(false, 0x00);
   
   //see if it talked
   if(PS2data[1] != 0x41 && PS2data[1] != 0x73 && PS2data[1] != 0x79){ //see if mode came back. If still anything but 41, 73 or 79, then it's not talking
      #ifdef PS2X_DEBUG
		/*Serial.println("Controller mode not matched or no controller found");
		Serial.print("Expected 0x41 or 0x73, got ");
		Serial.println(PS2data[1], HEX);*/
	  #endif
	 
	 return 1; //return error code 1
	}
  
  //try setting mode, increasing delays if need be. 
  read_delay = 1;
  
  for(int y = 0; y <= 10; y++)
  {
   sendCommandString(enter_config, sizeof(enter_config)); //start config run
   
   //read type
   	_delay_ms(CTRL_uint8_t_DELAY);

    //cli();                          // *** KJE *** disable for now
	SET(*_cmd_oreg,_cmd_mask);
    SET(*_clk_oreg,_clk_mask);
    CLR(*_att_oreg,_att_mask); // low enable joystick
    //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	
    _delay_ms(CTRL_uint8_t_DELAY);

    for (int i = 0; i<9; i++) {
	  temp[i] = _gamepad_shiftinout(type_read[i]);
    }

    //cli();                          // *** KJE *** disable for now
	SET(*_att_oreg,_att_mask); // HI disable joystick
    //SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	
	controller_type = temp[3];
   
   sendCommandString(set_mode, sizeof(set_mode));
   if(rumble){ sendCommandString(enable_rumble, sizeof(enable_rumble)); en_Rumble = true; }
   if(pressures){ sendCommandString(set_uint8_ts_large, sizeof(set_uint8_ts_large)); en_Pressures = true; }
   sendCommandString(exit_config, sizeof(exit_config));
   
   read_gamepad(false, 0x00);
   
   if(pressures){
	if(PS2data[1] == 0x79)
		break;
	if(PS2data[1] == 0x73)
		return 3;
   }
   
    if(PS2data[1] == 0x73)
      break;
      
    if(y == 10){
		#ifdef PS2X_DEBUG
		/*Serial.println("Controller not accepting commands");
		Serial.print("mode stil set at");
		Serial.println(PS2data[1], HEX);*/
		#endif
      return 2; //exit function with error
	  }
    
    read_delay += 1; //add 1ms to read_delay
  }
   
 return 0; //no error if here
}



 




uint8_t maskToBitNum(uint8_t mask) {
    for (int y = 0; y < 8; y++)
    {
      if(CHK(mask,y))
        return y;
    }
    return 0;
}


uint8_t readType(void) {
/*
	uint8_t temp[sizeof(type_read)];
	
	sendCommandString(enter_config, sizeof(enter_config));
	
	_delay_ms(CTRL_uint8_t_DELAY);

	SET(*_cmd_oreg,_cmd_mask);
    SET(*_clk_oreg,_clk_mask);
    CLR(*_att_oreg,_att_mask); // low enable joystick
	
    _delay_ms(CTRL_uint8_t_DELAY);

    for (int i = 0; i<9; i++) {
	  temp[i] = _gamepad_shiftinout(type_read[i]);
    }
	
	sendCommandString(exit_config, sizeof(exit_config));
	 
	if(temp[3] == 0x03)
		return 1;
	else if(temp[3] == 0x01)
		return 2;
	
	return 0;
	*/
	
	if(controller_type == 0x03)
		return 1;
	else if(controller_type == 0x01)
		return 2;
	
	return 0;
	
}

void enableRumble(void) {
  
     sendCommandString(enter_config, sizeof(enter_config));
     sendCommandString(enable_rumble, sizeof(enable_rumble));
     sendCommandString(exit_config, sizeof(exit_config));
     en_Rumble = true;
  
}

uint8_t enablePressures(void) {
  
     sendCommandString(enter_config, sizeof(enter_config));
     sendCommandString(set_uint8_ts_large, sizeof(set_uint8_ts_large));
     sendCommandString(exit_config, sizeof(exit_config));
	 
	 read_gamepad(false, 0x00);
	 read_gamepad(false, 0x00);
	 
	  if(PS2data[1] != 0x79)
		return false;
		
     en_Pressures = true;
	 return true;
}

void reconfig_gamepad(void){
  
   sendCommandString(enter_config, sizeof(enter_config));
   sendCommandString(set_mode, sizeof(set_mode));
   if (en_Rumble)
      sendCommandString(enable_rumble, sizeof(enable_rumble));
   if (en_Pressures)
      sendCommandString(set_uint8_ts_large, sizeof(set_uint8_ts_large));
   sendCommandString(exit_config, sizeof(exit_config));
   
}

/**************************************************************************************/


/*
bool Button(uint16_t);
unsigned int ButtonDatauint8_t();
bool NewButtonState();
bool NewButtonState(unsigned int);
bool ButtonPressed(unsigned int);
bool ButtonReleased(unsigned int);
void read_gamepad();
void read_gamepad(bool, uint8_t);
uint8_t readType();
uint8_t config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t config_gamepad(uint8_t, uint8_t, uint8_t, uint8_t, bool, bool);
void enableRumble();
bool enablePressures();
uint8_t Analog(uint8_t);
unsigned char _gamepad_shiftinout (char);
unsigned char PS2data[21];
void sendCommandString(uint8_t*, uint8_t);
void reconfig_gamepad();
unsigned char i;
unsigned int last_buttons;
unsigned int buttons;
uint8_t maskToBitNum(uint8_t);
uint8_t _clk_mask; 
volatile uint8_t *_clk_oreg;
uint8_t _cmd_mask; 
volatile uint8_t *_cmd_oreg;
uint8_t _att_mask; 
volatile uint8_t *_att_oreg;
uint8_t _dat_mask; 
volatile uint8_t *_dat_ireg;
unsigned long last_read;
uint8_t read_delay;
uint8_t controller_type;
bool en_Rumble;
bool en_Pressures;
*/
#endif



