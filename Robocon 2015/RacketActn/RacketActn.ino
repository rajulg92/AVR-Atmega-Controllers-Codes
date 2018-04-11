void setup()
{
  Serial.begin(57600);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
}
void loop()
{
Serial.println("go");
delay(2000);
Serial.println("o");

  //digitalWrite(5, LOW);
//  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  delay(500);
  digitalWrite(7, LOW);
  //digitalWrite(7, LOW);
  while(1);
  /*
  digitalWrite(5, HIGH);
  delay(100);
    */
}
