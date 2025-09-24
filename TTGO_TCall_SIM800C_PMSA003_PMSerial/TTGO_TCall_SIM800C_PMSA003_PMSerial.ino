#include <PMserial.h>
SerialPM pms(PMSx003, 21, 22);  // PMSx003, RX, TX

int LED_BUILTIN = 12;

void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  pms.init();                   // config serial port

  // LED Status
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  pms.read();                   // read the PM sensor
  Serial.print(F("PM1.0 "));Serial.print(pms.pm01);Serial.print(F(", "));
  Serial.print(F("PM2.5 "));Serial.print(pms.pm25);Serial.print(F(", "));
  Serial.print(F("PM10 ")) ;Serial.print(pms.pm10);Serial.println(F(" [ug/m3]"));

  // Chec PM2.5
  if (pms.pm25 >= 50) {
    // LED ON
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    // LED OFF
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(5000);                 // wait for 10 seconds
}
