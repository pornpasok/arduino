#include <Servo.h>
Servo myservo; 
void setup()
{
  Serial.begin(9600);   // used for print output
  myservo.attach(9);
}
void loop()
{
  myservo.write(0);
  Serial.println("0");
  delay(1000);
  myservo.write(90);
  Serial.println("90");
  delay(1000);
  myservo.write(180);
  Serial.println("180");
  delay(1000);
}
