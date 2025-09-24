#include <Wire.h>
#include <BH1750FVI.h>
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes); // สร้างออปเจกเซนเซอร์ BH1750
void setup()
{
  Serial.begin(9600);
  Serial.println("ArduinoAll TEST BH1750");
  LightSensor.begin(); // สั่งให้เซนเซอร์ เริ่มทำางน
}
void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity(); // อ่านค่าความเ้ข้มแสง หน่วยเป็น LUX
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  delay(1000);
}
