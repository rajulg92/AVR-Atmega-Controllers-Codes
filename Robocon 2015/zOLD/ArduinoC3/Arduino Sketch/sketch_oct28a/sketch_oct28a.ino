int InputData = 0;
int Led = 13; 

void setup()
{
  Serial.begin(9600);  
  pinMode(Led, OUTPUT);
  
}

void loop()
{
  while (Serial.available() > 0) 
  {
    InputData = Serial.read();
    Serial.write(InputData);
  }
  if (InputData == 'G') digitalWrite(Led, HIGH);   
  if (InputData == 'S') digitalWrite(Led, LOW);   

}
