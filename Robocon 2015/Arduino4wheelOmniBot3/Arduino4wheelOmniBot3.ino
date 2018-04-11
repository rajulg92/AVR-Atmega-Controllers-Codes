#include <PWM.h>
#include <Wire.h>
#include <GY80.h>
#include <PS2X_lib.h>
#include <Omni4.h>
#include <Rackets.h>
#include <Arduino.h>

#define TOGGLEBIT(P) *portInputRegister(digitalPinToPort(P)) = digitalPinToBitMask(P)

#define kp  6.5
#define kd  10
float ki = 0.0005;
Rackets Racket;
GY80 imu;
PS2X ps2x;
Omni omni(3,2,8,7,24,4,22,5,10,12,11,9);  //(FL, FR, BL, BR,FLD, FRD, BLD, BRD)    

int ps2Error=0;                                 
int LX = 0, LY = 0, Rot = 0;
int initialYaw = 0, error = 0, inst_err = 0, prev_err = 0, acc_err = 0, corr = 0;
boolean virgin = true;


//27,29
void setup()  {
  Serial.begin(57600);
  
  initRackets();
  omni.OmniInit();
  omni.OmniStop();

  ps2Error = ps2x.config_gamepad(38,42,40,44, false, false);  //(clock, command, attention, data, Pressures?, Rumble?)
  Serial.println(ps2Error);
  if(ps2Error == 1 || ps2Error == 2 )
    while(1);
  for (int i =0 ;i <10; i++)  ps2x.read_gamepad(false, 0);


  Wire.begin();
  init_imu();
  getRotations(10);
  initialYaw = imu.rotations.yaw;
  Serial.println(initialYaw);
}

void loop()  {
  ps2x.read_gamepad(false, 0);
  getRotations(1);
  LX = ps2x.Analog(PSS_LX);
  LY = ps2x.Analog(PSS_LY);
  if(ps2x.Button(PSB_GREEN))
  {
    
    if(virgin)
    {
      getRotations(10);
      initialYaw = imu.rotations.yaw;
      virgin = false;
    }
    error = calcError(imu.rotations.yaw)/2;
    error *=2;
    Serial.print(error);
    inst_err = error - prev_err;
    corr = kp*error + ki*acc_err + kd*inst_err;
    prev_err = error;
    acc_err += error;
    Serial.print(" :: ");
    Serial.println(corr);
    omni.OmniDrive( omni.Get_Distance(LX, LY), omni.Get_Angle(LX, LY)-45, -corr); 
  }
  else                                                                                                             
  {
    if(ps2x.Button(PSB_PAD_UP) && ps2x.Button(PSB_PAD_LEFT))        omni.OmniDrive(255,0,0);
    else if(ps2x.Button(PSB_PAD_UP) && ps2x.Button(PSB_PAD_RIGHT))    omni.OmniDrive(255,-90,0);
    else if(ps2x.Button(PSB_PAD_DOWN) && ps2x.Button(PSB_PAD_LEFT)) omni.OmniDrive(255,90,0);
    else if(ps2x.Button(PSB_PAD_DOWN) && ps2x.Button(PSB_PAD_RIGHT))  omni.OmniDrive(255,180,0);
    else if(ps2x.Button(PSB_PAD_UP))                                 omni.OmniDrive(255,-45,0);
    else if(ps2x.Button(PSB_PAD_LEFT))                              omni.OmniDrive(255,45,0);
    else if(ps2x.Button(PSB_PAD_RIGHT))                               omni.OmniDrive(255,-135,0);
    else if(ps2x.Button(PSB_PAD_DOWN))                               omni.OmniDrive(255,135,0);
    else
    {
      Rot = ps2x.Analog(PSS_RX) - 128;
      omni.OmniDrive( omni.Get_Distance(LX, LY), omni.Get_Angle(LX, LY)-45, Rot); 
      error=0; inst_err=0; prev_err=0; acc_err=0;
    }
  }
  
  
// int m1 =25, m2=23;
//  if(ps2x.Button(PSB_L1))
//  {
//    digitalWrite(m1,HIGH);
//    digitalWrite(m2,LOW);
//    
//  }
//  else if(ps2x.Button(PSB_R1))
//  {
//    digitalWrite(m1,LOW);
//    digitalWrite(m2,HIGH);
//    //Serial.println("22");
//  }  
//  else
//  {
//      digitalWrite(m1,LOW);
//      digitalWrite(m2,LOW);
//  }
//  
//  m1 =29, m2=27;
//  if(ps2x.Button(PSB_L2))
//  {
//    digitalWrite(m1,HIGH);
//    digitalWrite(m2,LOW);
//    
//  }
//  else if(ps2x.Button(PSB_R2))
//  {
//    digitalWrite(m1,LOW);
//    digitalWrite(m2,HIGH);
//    //Serial.println("22");
//  }  
//  else
//  {
//      digitalWrite(m1,LOW);
//      digitalWrite(m2,LOW);
//  }
//  Serial.print(Racket.calcAnalog(A5));
//  Serial.print("  ");
//  Serial.print(Racket.calcAnalog(A4));
// 
//  Serial.println("  ");
  
//  if(ps2x.Button(PSB_RED) || Racket.Rac11 == true )
//  {
//    Racket.AccRacket();
//  }
//  else
//  {
//    digitalWrite(Rac1M1,LOW);
//    digitalWrite(Rac1M2,LOW);
//    digitalWrite(Rac1pneu,HIGH);
//  }
//  
//  if(ps2x.Button(PSB_BLUE) || Racket.Rac21 == true )
//  {
//    Serial.println("here");
//    Racket.AccRacketService();
//    
//  }
//  else
//  {
//    digitalWrite(Rac2M1,LOW);
//    digitalWrite(Rac2M2,LOW);
//    digitalWrite(Rac2pneu,HIGH);
//  }
//  
//  if(ps2x.Button(PSB_START))
//  {
//    Racket.Service();
//    Racket.Rac21 = true;
//  } 

  if(ps2x.Button(PSB_PINK))
  {
    delay(100);
    TOGGLEBIT(Rac1pneu);
    TOGGLEBIT(Rac2pneu);
    delay(500);
  }  
  
//  if(digitalRead(33))
//  {
//   delay(72);
//   digitalWrite(Rac1pneu,HIGH);
//   digitalWrite(Rac2pneu,HIGH);
//  }
  
  if(ps2x.Button(PSB_RED))
  {
    digitalWrite(48,LOW);
    digitalWrite(46,HIGH);
    while(!digitalRead(33))	{};	
    delay(72);
    digitalWrite(Rac1pneu,HIGH);
    digitalWrite(Rac2pneu,HIGH);
    digitalWrite(46,LOW);
    digitalWrite(48,LOW);
  }
  if(ps2x.Button(PSB_R2))
  {
    digitalWrite(48,HIGH);
    digitalWrite(46,LOW);
  }
  
  Serial.print(digitalRead(Rac1pneu));
  Serial.print("  ");
  Serial.println(digitalRead(Rac2pneu));
  delay(2);  
}

void initRackets(void)
{
  
  pinMode(46, OUTPUT);	digitalWrite(46,LOW);
  pinMode(48, OUTPUT);	digitalWrite(48,LOW);
  pinMode(31,INPUT);      digitalWrite(31,LOW);
  pinMode(33,INPUT);      digitalWrite(33,LOW);
  
  pinMode(Rac1pneu, OUTPUT);	digitalWrite(Rac1pneu,HIGH);	
  pinMode(Rac1M1, OUTPUT);	digitalWrite(Rac1M1,LOW);	
  pinMode(Rac1M2, OUTPUT);	digitalWrite(Rac1M2,LOW);
  pinMode(Rac1Pot, INPUT);
	
  pinMode(Rac2pneu, OUTPUT);	digitalWrite(Rac2pneu,HIGH);
  pinMode(Rac2M1, OUTPUT);	digitalWrite(Rac2M1,LOW);
  pinMode(Rac2M2, OUTPUT);	digitalWrite(Rac2M2,LOW);
  pinMode(Rac2Pot, INPUT);
}


//-----------Standard Functions-------------------
void init_imu()  {
  imu.init_acc();
  imu.init_gyro();
  imu.init_mag();
}

void read_imu()  {
  imu.read_mag();
  imu.read_acc();
  imu.read_gyro();
}

void getRotations(int Samples)  {
  float _GRa = 0, _GRb = 0, _GRc = 0;
  float gravity[3], geomagnetic[3];
  for(int SampleTimes = 0; SampleTimes<Samples; SampleTimes++ )	{
    read_imu();
    geomagnetic[0] = imu.mag.x;
    geomagnetic[1] = imu.mag.y;
    geomagnetic[2] = imu.mag.z;
    gravity[0] = imu.acc.x;
    gravity[1] = imu.acc.y;
    gravity[2] = imu.acc.z;
    imu.getRotation(gravity, geomagnetic);
    _GRa+=imu.rotations.yaw;
    _GRb+=imu.rotations.pitch;
    _GRc+=imu.rotations.roll;
  }
  imu.rotations.yaw = _GRa/Samples;
  imu.rotations.pitch = _GRb/Samples;
  imu.rotations.roll = _GRc/Samples; 
}

int calcError(int c)
{
  c -= initialYaw;
  if(c>180)
    return (c - 360);
  else
   return c;
}
