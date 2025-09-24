//extern "C" {
//#include "user_interface.h"
//}

ADC_MODE(ADC_VCC);

int vdd = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println();
  vdd = ESP.getVcc();
  Serial.println(vdd);
}

void loop() {
  vdd = ESP.getVcc();
  Serial.println(vdd);
  delay(5000);
}
