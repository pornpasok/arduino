#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "SookYenFarm";
const char* password = "0863741677";
const char* host = "https://gpstracking.tono.tech/post-data.php"; 
const char* api   = "tono.tech";  //API Key

int LED_BUILTIN = 2;
uint32_t chipId = 0;

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXPin (22)
#define TXPin (21)

static const uint32_t GPSBaud = 9600;
float latitude, longitude, speed;
String lat_str, lng_str, spd_str;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30        /* Time ESP will go to sleep (in seconds) */

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial ss(2);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  ss.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin, false);
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();
  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    // LED Status
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void getdata(String lat_str, String lng_str, String spd_str) {
 
    HTTPClient https;
    https.begin(host);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Chip ID
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    Serial.print("Chip ID: ");
    Serial.println(chipId);

//    Serial.print("Latitude= "); 
//    Serial.print(lat_str);
//    Serial.print(" Longitude= "); 
//    Serial.print(lng_str);
//    Serial.print(" Speed= "); 
//    Serial.println(spd_str);


    String httpRequestData = "api_key=";
    httpRequestData += api;
    httpRequestData += "&gpsid=";
    httpRequestData += chipId;
    httpRequestData += "&lat=";
    httpRequestData += lat_str;
    httpRequestData += "&lng=";
    httpRequestData += lng_str;
    httpRequestData += "&speed=";
    httpRequestData += spd_str;

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN, HIGH);
    
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      //String httpRequestData = "api_key=tono.tech&gpsid=2580252&lat=13.831049&lng=100.572044&speed=14.667840";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      
      int httpCode = https.POST(httpRequestData);
      // httpCode will be negative on error
      if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
       Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
      delay(1000);
      Serial.println("LED OFF");
      digitalWrite(LED_BUILTIN, LOW);
  
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

      latitude = gps.location.lat();
      lat_str = String(latitude , 6);

      longitude = gps.location.lng();
      lng_str = String(longitude , 6);

      speed = gps.speed.kmph();
      spd_str = String(speed , 6);
      
      getdata(lat_str, lng_str, spd_str);
      Serial.print("Sleeping ");
      Serial.print(TIME_TO_SLEEP);
      Serial.println(" seconds ..");
      // Deep Sleep TIME_TO_SLEEP seconds
      ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
      //delay(30000);
    }
  }
  
}
