#include <Wire.h>
#include <GY80.h>
GY80 imu;
int i = 0;
void setup()
{
  Serial.begin(57600);
  Wire.begin();
  init_imu();
  getRotations(10);
  i = imu.rotations.yaw;
  Serial.println("inti");

}

void loop()
{
  getRotations(1);
  Serial.println(calcError(imu.rotations.yaw));
  
  /*
  if (i> 300)
  {
    if(c>180 )
    {
      Serial.print(c-i);
    }
    else
    {
      Serial.print((360-i)+c);
    }
  }
  else if(i < 60)
  {
    if(c > 180)
    {
      Serial.print(c-360-i);
    }
    else
    { 
      Serial.print(c-i);
    }
  }
  else
  {
    Serial.print(c-i);
  }*/
  
  //while(1);
}

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
  c -= i;
  Serial.print(i);
  Serial.print(": :");
  Serial.print(c);
  Serial.print(": :");
  if(c>180)
    return (c - 360);
  else
    return c;
}
