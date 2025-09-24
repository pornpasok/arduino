#include <SoftwareSerial.h> // using SoftwareSerial might create problems in activating sleep mode, see additional Serial.flush(); below
#include "PMS.h"
#include <esp_task_wdt.h>

SoftwareSerial serialDust(18, 19); // RX, TX

PMS pms(serialDust);
PMS::DATA data;

#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>
#include "SHT21.h"

SHT21 SHT21;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  265        /* Time ESP will go to sleep (in seconds) */
#define WDT_TIMEOUT 120         //All process should be finished within 2 minutes.

const char* ssid     = "SookYenFarm";    // SSID Wifi
const char* password = "0863741677";   // Password Wifi
const char* host = "https://dashboard.sookyenfarm.com/esp-post-data.php";
const char* api   = "tonofarm.io";  //API Key
String sensorLocation = "13.8306416,100.5714156";

long timeout; // Time to WiFi Connect

// RSSI (WiFi Signal)
long rssi;

// ESP32 ChipID
uint32_t chipId = 0;

int analogInPin  = 34;    // Analog input pin
int sensorValue;
float calibration = -0.75; // Check Battery voltage using multimeter & add/subtract the value


int LED_BUILTIN = 2;

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

  // Chip ID
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  Serial.print("Chip ID: ");
  Serial.println(chipId);

 
  float vbatt = 0;
  sensorValue = analogRead(analogInPin);
  //vbatt = (((sensorValue * 3.3) / 1024) / 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor
  vbatt = (((sensorValue*4.2)/(1024*2)) + calibration) ; //multiply by two as voltage divider network is 100K & 100K Resistor
  
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
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("--------------------");
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT21.getHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT21.getTemperature());

  float t = SHT21.getTemperature();
  float h = SHT21.getHumidity();

  Serial.println("Waking up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  for (int i=1; i<=30; i++) {
    Serial.println(i);
    if (i>=27) {
        for (int j=1; j<=5; j++) {
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("LED OFF");
          delay(150);
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("LED ON");
          delay(50);   
        }
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED OFF");
        delay(900);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED ON");
        delay(100);   

      }
  }
  //delay(30000);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");

  Serial.println("Send read request...");
  pms.requestRead();
  
  if (pms.readUntil(data)) {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    Serial.println();
  } else {
    Serial.println("No data.");
  }

  Serial.println("PMS Going to sleep.");
  Serial.flush(); // IMPORTANT when using software serial
  pms.sleep();


  
  Serial.print("connecting to ");
  Serial.println(host);

  // Show Connect Status
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);
  Serial.println("LED ON");

  HTTPClient https;
  https.begin(host);

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
  esp_task_wdt_init(WDT_TIMEOUT, true); 
  esp_task_wdt_add(NULL);
  
  SHT21.begin();
  // Serial Port setups
  Serial.begin(9600);   // used for print output
  

  //Serial.begin(9600);
  //Serial.setTimeout(2000);
  // LED Status
  pinMode(LED_BUILTIN, OUTPUT);
  
  serialDust.begin(9600); // software serial for dust sensor
  pms.passiveMode();    // Switch to passive mode

  // Connect WiFi and Send Data
  connect();
  
  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");
  // Deep Sleep TIME_TO_SLEEP seconds
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
}

void loop()
{
  
}
