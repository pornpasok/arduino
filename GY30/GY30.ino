#include <BH1750FVI.h>
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  LightSensor.begin();
}
 
void loop() {
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  if (lux < 1000) {
    digitalWrite(LED_BUILTIN, HIGH); // สั่งให้ LED ติดสว่าง
    Serial.println("LEDON");
  }
  if (lux > 1000) {
    digitalWrite(LED_BUILTIN, LOW); // สั่งให้ LED ดับ
    Serial.println("LEDOFF");
  }
  delay(1000);
}
