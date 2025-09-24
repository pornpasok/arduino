#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char *ssid = "SookYenFarm";
const char *password = "0863741677";
const char *host = "http://gpstracking.tono.tech/post-data.php";  
const char* api   = "tono.tech";  //API Key
long timeout; // Time to WiFi Connect
String gpsid = "888888";

#include <WiFiClient.h>
WiFiClient client;

// GPS-MODUL
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial ss(5, 4);  // GPIO4 und GPIO5 (PIN D1 und D2)

float latitude, longitude, speed;
String lat_str, lng_str, spd_str;


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  58        /* Time ESP will go to sleep (in seconds) */


void connect() {
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    // LED Status
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    //delay(500);
    Serial.print(".");
    timeout = millis();
    Serial.println(timeout);

    // Time Out got deep_sleep_mode save battery
    if (timeout > 30000) {
      Serial.print("TIME_OUT: ");
      Serial.println(timeout);
      Serial.print("Sleeping ");
      Serial.print(TIME_TO_SLEEP);
      Serial.println(" seconds ..");
      // Deep Sleep TIME_TO_SLEEP seconds
      ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("--------------------");

  // Show Connect Status
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);
  Serial.println("LED ON");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial:
  Serial.begin(9600);
  // SoftwareSerial:
  ss.begin(9600);
  connect();
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
      
     
      
      HTTPClient https;
      https.begin(client, host);

      // Specify content-type header
      https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      //postData = "api_key=" + apiKeyValue + "&gpsid=" + gpsid + "&lat=" + lat + "&lng=" + lng + "&speed=" + spd;postData = "api_key=" + apiKeyValue + "&gpsid=" + gpsid + "&lat=" + lat + "&lng=" + lng + "&speed=" + spd;
      // We now create a URI for the request
      String httpRequestData = "api_key=";
      httpRequestData += api;
      httpRequestData += "&gpsid=";
      httpRequestData += gpsid;
      httpRequestData += "&lat=";  
      httpRequestData += lat_str;
      httpRequestData += "&lng=";  
      httpRequestData += lng_str;
      httpRequestData += "&speed=";  
      httpRequestData += spd_str;
      
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
    
      // Send HTTP POST request
      int httpResponseCode = https.POST(httpRequestData);
    
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      Serial.println(https.getString());

      // Free resources
      https.end();
      //delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED OFF");
      delay(60000);
    }
    
    //delay(1000);
    
  }
}
