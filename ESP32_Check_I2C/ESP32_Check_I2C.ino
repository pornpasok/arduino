#include <Wire.h>
#include <SHT21.h>
SHT21 sht;

float t;
float h;
float p;

byte error;
byte address = 64; // SHT21 0x40
 
void setup() {
  Wire.begin(18,19);
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}

void loop() {
  
  // Temp (SHT21)
  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  Serial.print("I2C Error Code:");
  Serial.println(error);

      // Check Error Code
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);

        t = sht.getTemperature();
        h = sht.getHumidity();
    
        Serial.print("Temperature(C): ");
        Serial.println(t);
        Serial.print("Humidity(%RH): ");
        Serial.println(h);
        Serial.println("--------------------");
      }
      else if (error==1) {
        Serial.println("Please check I2C cable");
        // Random SHT21 Sensor Value for Testing
        t = random(20, 30);
        h = random(50, 100);
      }    
      else if (error==2) {
        Serial.println("Please check I2C cable");
        // Random SHT21 Sensor Value for Testing
        t = random(20, 30);
        h = random(50, 100);
      }    
      else if (error==3) {
        Serial.println("Please check Power cable");
        // Random SHT21 Sensor Value for Testing
        t = random(20, 30);
        h = random(50, 100);
      }    
      else if (error==4) {
        Serial.print("Unknow error at address 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address,HEX);
      }    

    
  delay(5000);
}
