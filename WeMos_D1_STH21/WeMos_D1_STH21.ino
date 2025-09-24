#include <Wire.h>
#include <Sodaq_SHT2x.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
}
 
void loop() {
  Serial.print("Temperature(C): ");
  Serial.println(SHT2x.GetTemperature());
  Serial.print("Humidity(%RH): ");
  Serial.println(SHT2x.GetHumidity());
  Serial.print("Dewpoint(C): ");
  Serial.println(SHT2x.GetDewPoint());
  Serial.println("--------------------");
   
  delay(5000);
}
