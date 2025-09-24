#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <SoftwareSerial.h>
#include "PMS.h"
// #include <esp_task_wdt.h>

SoftwareSerial serialDust(12, 14); // RX, TX

PMS pms(serialDust);
PMS::DATA data;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  275        /* Time ESP will go to sleep (in seconds) */
#define WDT_TIMEOUT 120         //All process should be finished within 2 minutes.

const char* ssid     = "SookYenFarm";    // SSID Wifi
const char* password = "0863741677";   // Password Wifi
const char* host = "https://dashboard.sookyenfarm.com/esp-post-data.php";
const char* api   = "tonofarm.io";  //API Key
String sensorLocation = "13.8306416,100.5714156";

long timeout; // Time to WiFi Connect
long rssi; // RSSI (WiFi Signal)


float t = 0;
float h = 0;

// Chip ID
unsigned long chipId = ESP.getChipId();

//// VBatt
//ADC_MODE(ADC_VCC); // ESP8266 Only
//float vbatt;

int analogInPin  = A0;    // Analog input pin
int sensorValue;
float calibration = 0.07; // Check Battery voltage using multimeter & add/subtract the value

float set_temp_offset = 4.80; 
float set_rhum_offset = 7.00;


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
    if (timeout > 60000) {
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

  // PMS5003T
  Serial.println("Waking up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  //delay(30000);

  for (int i=1; i<=30; i++) {
      Serial.println(i);
 
      if (i>=27) {
        for (int j=1; j<=5; j++) {
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("LED OFF");
          delay(150);
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("LED ON");
          delay(50);   
        }
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED OFF");
        delay(900);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED ON");
        delay(100);   

      }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED OFF");
  

  Serial.println("Send read request...");
  pms.requestRead();

  Serial.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);
    
    Serial.println(data.AMB_TMP);
    Serial.println(data.AMB_HUM);

    t = data.AMB_TMP/10.2F + set_temp_offset;
    h = data.AMB_HUM/10.2F + set_rhum_offset;
    
    Serial.print("Temperature (C): ");
    Serial.println(t);

    Serial.print("Humidity (%): ");
    Serial.println(h);
  }
  else
  {
    Serial.println("No data.");
    Serial.print("Sleeping ");
    Serial.print(TIME_TO_SLEEP);
    Serial.println(" seconds ..");
    // Deep Sleep TIME_TO_SLEEP seconds
    ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  }

  // ChipID
  Serial.print("ChipID: ");
  Serial.println(chipId);

  // VCC
//  vbatt = ESP.getVcc() / 1000.0F;
//  Serial.print("VCC: ");
//  Serial.println(vbatt);
    
  float vbatt = 0;
  sensorValue = analogRead(analogInPin);
  vbatt = (((sensorValue*3.3)/1024)*2 + calibration) ; //multiply by two as voltage divider network is 100K & 100K Resistor
  
  // VCC
  Serial.print("Analog Read: ");
  Serial.println(sensorValue);
  Serial.print("VBATT: ");
  Serial.println(vbatt);

  // Wi-Fi Signal
  rssi = WiFi.RSSI();
  Serial.print("WiFi Signal: ");
  Serial.println(rssi);
  Serial.println("--------------------");

  Serial.println("Going to sleep for 60 seconds.");
  pms.sleep();
  // delay(60000);
  

  // Connect Host
  Serial.print("connecting to ");
  Serial.println(host);

  // Show Connect Status
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);
  Serial.println("LED ON");

  WiFiClientSecure client;
  client.setInsecure(); //the magic line, use with caution
  client.connect(host, 443);
  
  HTTPClient https;
  https.begin(client, host);

  // Specify content-type header
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // We now create a URI for the request
  String httpRequestData = "api_key=";
  httpRequestData += api;
  httpRequestData += "&sensor=";
  httpRequestData += chipId;
  httpRequestData += "&location=";  
  httpRequestData += sensorLocation;
  httpRequestData += "&value1=";  
  httpRequestData += t;
  httpRequestData += "&value2=";  
  httpRequestData += h;
  httpRequestData += "&value3=";  
  httpRequestData += data.PM_AE_UG_1_0;
  httpRequestData += "&value4=";  
  httpRequestData += data.PM_AE_UG_2_5;  
  httpRequestData += "&value5=";
  httpRequestData += data.PM_AE_UG_10_0;
  //httpRequestData += vbatt;
  httpRequestData += "&value6=";  
  //httpRequestData += rssi;
  httpRequestData += vbatt;
  
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
  // Free resources
  https.end();
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);
  Serial.println("LED OFF");
}

void setup()
{
  //Config WDT
//  esp_task_wdt_init(WDT_TIMEOUT, true); 
//  esp_task_wdt_add(NULL);
  
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  serialDust.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  pinMode(LED_BUILTIN, OUTPUT);
  pms.passiveMode();    // Switch to passive mode

  // Connect WiFi and Send Data
  connect();
  
  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");
  //delay(6000);
  // Deep Sleep TIME_TO_SLEEP seconds
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
  
}

void loop()
{
  
}
