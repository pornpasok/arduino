#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN D1   // ขา Out ของ Sensor ต่อเข้าขา D4 ของ Esp8266
//เลือกชนิดของ Sensor
//#define DHTTYPE DHT11     //DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "TonMiA1";    // SSID Wifi
const char* password = "0863741677";   // Password Wifi
const char* host = "api.thingspeak.com";
const char* api   = "IY382WX2SP3JQAA1";  //API Key ที่เราจำไว้ ในขั้นต้นเมื่อกี้

void setup() {
  Serial.begin(9600);
  delay(10);
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
  Serial.println("DHTxx test!");
  dht.begin();
}

int value = 0;
void loop() {
  delay(60000);
  ++value;
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
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/update?api_key=";
  url += api;
  url += "&field1=";
  url += t;
  url += "&field2=";  
  url += h;
  // เราจะส่งข้อมูล https://api.thingspeak.com/update?api_key=RRHS37ETW76RFAWB&field1=(อุณหภูมิ)&field2=(ความชื่น)
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
   //Wait up to 10 seconds for server to respond then read response
   int i=0;
   while((!client.available()) && (i<1000)){
     delay(10);
     //Serial.println(i);
     i++;
   }
   while(client.available()){
     String line = client.readStringUntil('\r');
     Serial.print(line);
   }
   Serial.println();
   Serial.println("closing connection");
}
