
#include <Wire.h>
#include "SHT21.h"

SHT21 SHT21;

void setup()
{
  Wire.begin(18,19);
  SHT21.begin();
  Serial.begin(115200);
}

void loop()
{
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT21.getHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT21.getTemperature());
  
  delay(1000);
}
