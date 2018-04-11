#include <PWM.h>
#include <Wire.h>
#include <GY80.h>
#include <PS2X_lib.h>
#include <Omni4.h>
#include <Arduino.h>

#define TOGGLEBIT(P) *portInputRegister(digitalPinToPort(P)) = digitalPinToBitMask(P)

//GY80 imu;
PS2X ps2x;
Omni omni(9,11,7,8, 49,53,47,51,41,45,39,43);  //(FL, FR, BL, BR,FLD, FRD, BLD, BRD)    


int error=0;                                 
int LX = 0, LY = 0, Rot = 0;
//int initialYaw = 0, currentYaw = 0;

void setup()  {
  Serial.begin(57600);

  omni.OmniInit();
  omni.OmniStop();

  error = ps2x.config_gamepad(46,50,48,52, false, false);//(clock, command, attention, data, Pressures?, Rumble?)
  Serial.println(error);
  if(error == 1 || error == 2 )
    while(1);
  for (int i =0 ;i <10; i++)  ps2x.read_gamepad(false, 0);


  /*Wire.begin();
  init_imu();
  getRotations(10);
  initialYaw = imu.rotations.yaw;*/
}

void loop()  {
  ps2x.read_gamepad(false, 0);
  //getRotations(1);
  LX = ps2x.Analog(PSS_LX);
  LY = ps2x.Analog(PSS_LY);
  Rot = ps2x.Analog(PSS_RX) - 128;
  omni.OmniDrive( omni.Get_Distance(LX, LY), omni.Get_Angle(LX, LY)-45, -Rot); 
  delay(1);  
}

//-----------Standard Functions-------------------
/*void init_imu()  {
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
}*/
