#include <PS2X_lib.h>
#define TOGGLEBIT(P) *portInputRegister(digitalPinToPort(P)) = digitalPinToBitMask(P)
PS2X ps2x;

int m1 = 27, m2 = 29,pot= A5;
int ps2Error=0;
void setup()
{
  delay(1000);
  Serial.begin(57600);
  ps2Error = ps2x.config_gamepad(38,42,40,44, false, false);  //(clock, command, attention, data, Pressures?, Rumble?)
  Serial.println(ps2Error);
  if(ps2Error == 1 || ps2Error == 2 )
    while(1);
  for (int i =0 ;i <10; i++)  ps2x.read_gamepad(false, 0);
  
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(pot,INPUT);
  pinMode(46,OUTPUT);
  pinMode(48,OUTPUT);
  pinMode(33,INPUT);
  digitalWrite(46,HIGH);  //L293D
  digitalWrite(48,LOW);
    
}

void loop()
{
  ps2x.read_gamepad(false, 0);
  Serial.println(analogRead(pot)); 
  if(ps2x.Button(PSB_RED))
  {
   // while(!digitalRead(33));
    while(analogRead(pot) < 600 )
    {
      digitalWrite(m1,HIGH);
      digitalWrite(m2,LOW);
    }
    while(analogRead(pot) > 210)
    {
      digitalWrite(m1,LOW);
      digitalWrite(m2,HIGH);
    }
    digitalWrite(m1,HIGH);
    digitalWrite(m2,HIGH);
  }
  if(ps2x.Button(PSB_R1))
  {
    digitalWrite(m1,LOW);
    digitalWrite(m2,HIGH);
  }
  else
  {
    digitalWrite(m1,LOW);
    digitalWrite(m2,LOW);
  }
  delay(2);
}
