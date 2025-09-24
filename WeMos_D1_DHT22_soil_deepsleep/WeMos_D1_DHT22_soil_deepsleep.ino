#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"
#//define SECONDS_DS(seconds) ((seconds)*1000000UL)
#define DHTPIN D3   // NodeMCU PIN D1
//#define DHTTYPE DHT11     //DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  598        /* Time ESP32 will go to sleep (in seconds) */

DHT dht(DHTPIN, DHTTYPE);

const int AirValue = 741;   //you need to replace this value with Value_1
const int WaterValue = 301;
int s = 0;
int spercent = 0;


ADC_MODE(ADC_VCC);

const char* ssid     = "tono.io";    // SSID Wifi
const char* password = "0863741677";   // Password Wifi
const char* host = "http://tonofarm.herokuapp.com/esp-post-data.php";
const char* api   = "tonofarm.io";  //API Key
String sensorLocation = "12.7581423,102.1468503";

float v;

void connect() {
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Check timeout
  unsigned long timeout = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    timeout = millis();
    Serial.println(timeout);

    // Time Out got deep_sleep_mode save battery
    if (timeout > 30000) {
      Serial.print("TIME_OUT: ");
      Serial.println(timeout);
      Serial.println("Sleeping 10 minutes ..");
      ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    }
    
    
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Temperature
  dht.begin();

  // LED Stop
  digitalWrite(D4, HIGH);
  delay(10);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

   // Soil Moisture Sensor
  /*
  int s = analogRead(A0);
  Serial.print("Soil Moisture: ");
  Serial.println(s);
  spercent = map(s, AirValue, WaterValue, 0, 100);
  if(spercent >= 100) {
    Serial.println("100 %");
  }
  else if(spercent <=0) {
    Serial.println("0 %");
  }
  else if(spercent >0 && spercent < 100) {
    Serial.print(spercent);
    Serial.println("%");
  }
  */
  
  // VCC
  v = ESP.getVcc() / 1000.0;
  Serial.print("VCC: ");
  Serial.println(v);
  
  Serial.print("connecting to ");
  Serial.println(host);

  HTTPClient http;
  http.begin(host);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // We now create a URI for the request
  String httpRequestData = "api_key=";
  httpRequestData += api;
  httpRequestData += "&sensor=";
  httpRequestData += ESP.getFlashChipId();
  httpRequestData += "&location=";  
  httpRequestData += sensorLocation;
  httpRequestData += "&value1=";  
  httpRequestData += t;
  httpRequestData += "&value2=";  
  httpRequestData += h;
  httpRequestData += "&value3=";  
  httpRequestData += s;
  httpRequestData += "&value4=";  
  httpRequestData += v;
  
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
  
  // Show Connect Status
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void setup(void) {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  delay(10);
  pinMode(D4, OUTPUT);

  // Wait for serial to initialize.
  //while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  connect();

  Serial.println("Sleeping 10 minutes ..");
  // Deep Sleep 60 seconds
  //ESP.deepSleep(SECONDS_DS(59));
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
  //delay(60000);
  //exit(0);
}

void loop() {
}
