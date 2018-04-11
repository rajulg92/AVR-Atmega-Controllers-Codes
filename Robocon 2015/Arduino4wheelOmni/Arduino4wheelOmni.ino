#include <PWM.h>
#include <Wire.h>
#include <GY80.h>
#include <PS2X_lib.h>
#include <Omni4.h>
#include <Arduino.h>

#define TOGGLEBIT(P) *portInputRegister(digitalPinToPort(P)) = digitalPinToBitMask(P)

#define kp  6.5
#define kd  10
float ki = 0.0005;

GY80 imu;
PS2X ps2x;
Omni omni(9,8,11,12,36,40,38,42,34,28,32,30);  //(FL, FR, BL, BR,FLD, FRD, BLD, BRD)    

int ps2Error=0;                                 
int LX = 0, LY = 0, Rot = 0;
int initialYaw = 0, error = 0, inst_err = 0, prev_err = 0, acc_err = 0, corr = 0;
boolean virgin = true;

boolean Rac1 = true, Rac11 = false;

void setup()  {
  pinMode(25,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(37,OUTPUT);  digitalWrite(37,HIGH);
  pinMode(A1, INPUT);  digitalWrite(A1,LOW);
  pinMode(A2, INPUT);  digitalWrite(A2,LOW);
  
  Serial.begin(57600);

  omni.OmniInit();
  omni.OmniStop();

  ps2Error = ps2x.config_gamepad(46,50,52,48, false, false);//(clock, command, attention, data, Pressures?, Rumble?)
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
    Rot = ps2x.Analog(PSS_RX) - 128;
    omni.OmniDrive( omni.Get_Distance(LX, LY), omni.Get_Angle(LX, LY)-45, Rot); 
    error=0; inst_err=0; prev_err=0; acc_err=0;
  }
 /*
  if(ps2x.Button(PSB_L1))
  {
    digitalWrite(25,HIGH);
    digitalWrite(27,LOW);
    
  }
  else if(ps2x.Button(PSB_R1))
  {
    digitalWrite(25,LOW);
    digitalWrite(27,HIGH);
    //Serial.println("22");
  }  
  else
  {
    if(!Rac11)
    {
      digitalWrite(25,LOW);
      digitalWrite(27,LOW);
    }
  }
  if(ps2x.Button(PSB_RED) || Rac11 == true )
  {
    AccRacket();
  }
  else
  {
    digitalWrite(25,LOW);
    digitalWrite(27,LOW);
    digitalWrite(37,HIGH);
  }
  */
  
  Serial.print(calcAnalog(A6));
  Serial.print("  ");
  Serial.print(digitalRead(A2));
  Serial.print("  ");
  Serial.println(digitalRead(A1));
  delay(1);  
}

void AccRacket()
{
  if(Rac1)
  {
    Rac11 = true;
    digitalWrite(37, LOW); //pneu off
    if(calcAnalog(A6) > 1.74)//motot cw till 1.22
    {
       digitalWrite(25,LOW);
       digitalWrite(27,HIGH);  //motor back
    }
    else
    {
      digitalWrite(25,LOW);
      digitalWrite(27,LOW);
      Rac1 = false;
    }
    Serial.println("11");
  }
  if(!Rac1)
  {
    
    digitalWrite(37, HIGH); //pneu on
    if(calcAnalog(A6) < 2.12)
    {
      digitalWrite(25,HIGH);
      digitalWrite(27,LOW);//motor aaagee
    }
    else
    {
      digitalWrite(25,LOW);
      digitalWrite(27,LOW);
      Rac1 = true;
      Rac11 = false;
    }
    Serial.println("22");
  }  
}

float calcAnalog(uint8_t pin)
{
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (5.0 / 1023.0);// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  return voltage; //return the value you read:
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
