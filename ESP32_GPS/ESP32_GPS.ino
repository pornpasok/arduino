#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXPin (22)
#define TXPin (21)

static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial ss(2);

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin, false);
  Serial.println(TinyGPSPlus::libraryVersion());
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.print(gps.location.lng(), 6);
      Serial.print(" Speed= "); 
      Serial.println(gps.speed.kmph(), 6);
      delay(1000);
    }
  }
}
