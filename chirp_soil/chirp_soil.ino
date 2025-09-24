#include <Wire.h>
const int AirValue = 275;   //you need to replace this value with Value_1
const int WaterValue = 523;

void setup() {
  Wire.setClockStretchLimit(4000);
  Wire.begin();
  Serial.begin(9600);
}

void writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(1100);
  Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

void loop() {
  Serial.print(readI2CRegister16bit(0x20, 0)); //read capacitance register
  writeI2CRegister8bit(0x20, 3); //request light measurement 
  delay(9000);                   //this can take a while
  Serial.print(", ");
  Serial.println(readI2CRegister16bit(0x20, 4)); //read light register
  delay(500);
}
