#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
  
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

ADC_MODE(ADC_VCC);
float v;

const char* ssid = "tono.io";
//const char* ssid = "SookYenFarm";
const char* password = "0863741677";

const char* serverName = "http://tonofarm.herokuapp.com/esp-post-data.php";

String apiKeyValue = "tonofarm.io";
//uint64_t chipid = ESP.getFlashChipId();
//String sensorName = chipid;
String sensorLocation = "12.7581423,102.1468503";

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP280 bmp;  // I2C
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

int value = 0;
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
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // VCC
  v = ESP.getVcc() / 1000.0;
  Serial.print("VCC: ");
  Serial.println(v);
  
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bmp.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BMEP280 sensor, check wiring or change I2C address!");
    while (1);
  }
  
  // Use WiFiClient class to create TCP connections
  
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepare your HTTP POST request data
      //String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + ESP.getFlashChipId()
      //                      + "&location=" + sensorLocation + "&value1=" + String(bmp.readTemperature())
      //                      + "&value2=" + String(bmp.readHumidity()) + "&value3=" + String(bmp.readPressure()/100.0F) + "&value4=" + v + "";
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + ESP.getFlashChipId()
                            + "&location=" + sensorLocation + "&value1=" + String(bmp.readTemperature())
                            + "&value2=" + 0 + "&value3=" + String(bmp.readPressure()/100.0F) + "&value4=" + v + "";
      
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      // Show Connect Status
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(1000);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

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
   else {
    Serial.println("WiFi Disconnected");
   }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.setTimeout(2000);
  // Wait for serial to initialize.
  while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  connect();

  delay(1000);

  Serial.println("Sleeping 598 seconds ..");
  // Deep Sleep 600 seconds
  ESP.deepSleep(598e6); // 60e6 is 60 microsecondsESP.
}

void loop() {
}
