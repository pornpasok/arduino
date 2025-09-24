#include <Wire.h>
#include <SHT21.h>

SHT21 sht;

#define I2C_SDA_2 18
#define I2C_SCL_2 19

#define SEALEVELPRESSURE_HPA (1013.25)

TwoWire I2CSHT = TwoWire(0);

unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  Serial.println(F("SHT21 test"));
  
  Wire.begin(I2C_SDA_2, I2C_SCL_2);  
  
  byte error, address;
  address = 64; // SHT21 0x40
  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  Serial.print("Error Code:");
  Serial.println(error);
  if (error == 0) {
    Serial.print("I2C device found at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    Serial.println(address,HEX);
  }
  else if (error==2) {
    Serial.println("Please check I2C cable");
  }   
  else if (error==3) {
    Serial.println("Please check I2C cable");
  }   
  else if (error==4) {
    Serial.print("Unknow error at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    Serial.println(address,HEX);
  }    
   

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}

void loop() { 
  printValues();
  delay(delayTime);
}

void printValues() {
  
  Serial.print("Temperature = ");
  Serial.print(sht.getTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

  Serial.print("Humidity = ");
  Serial.print(sht.getHumidity());
  Serial.println(" %");

  Serial.println();
}
