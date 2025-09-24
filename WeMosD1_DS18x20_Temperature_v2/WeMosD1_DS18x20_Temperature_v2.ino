#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 // D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59        /* Time ESP will go to sleep (in seconds) */
#define SEALEVELPRESSURE_HPA (1013.25)
long timeout; // Time to WiFi Connect

const char* ssid     = "SookYenFarm";    // SSID Wifi
const char* password = "0863741677";   // Password Wifi
const char* host = "https://dashboard.sookyenfarm.com/esp-post-data.php";
const char* api   = "tonofarm.io";  //API Key
String sensorLocation = "12.7581423,102.1468503";

// VBatt
ADC_MODE(ADC_VCC); // ESP8266 Only
float vbatt;

// Chip ID
unsigned long chipId = ESP.getChipId();
//uint32_t chipId = ESP.getEfuseMac();

// RSSI (WiFi Signal)
long rssi;

void connect() {
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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

  // Temp
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures(); //Read from library
  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0)); // Temperature
  Serial.println(" *C");

  float t = sensors.getTempCByIndex(0);

  // Chip ID
  Serial.print("Chip ID: ");
  Serial.println(chipId);
  
  // VCC
  vbatt = ESP.getVcc() / 1000.0F;
  Serial.print("VCC: ");
  Serial.println(vbatt);

  // Wi-Fi Signal
  rssi = WiFi.RSSI();
  Serial.print("WiFi Signal: ");
  Serial.println(rssi);
  
  Serial.print("connecting to ");
  Serial.println(host);

  //HTTPClient http;
  HTTPClient https;
  WiFiClient client;
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
  httpRequestData += 0;
  httpRequestData += "&value3=";  
  httpRequestData += 0;
  httpRequestData += "&value4=";  
  httpRequestData += rssi;
  httpRequestData += "&value5=";  
  httpRequestData += vbatt;
  httpRequestData += "&value6=";  
  httpRequestData += 0;
  
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = https.POST(httpRequestData);
  
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
  
  while (client.available())
   {
    char c = client.read();
    Serial.print(c);
  }
  
  // Free resources
  https.end();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void setup(void) {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  delay(10);
  pinMode(D4, OUTPUT);
  Serial.println("Dallas Temperature IC Control Library");
  sensors.begin();

  // Wait for serial to initialize.
  //while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  connect();

  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");
  // Deep Sleep TIME_TO_SLEEP seconds
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 60e6 is 60 microsecondsESP.
}

void loop() {
}
