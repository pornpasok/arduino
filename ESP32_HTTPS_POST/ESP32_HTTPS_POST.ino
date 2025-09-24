#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "SookYenFarm";
const char* password = "0863741677";
const char* host = "https://gpstracking.tono.tech/post-data.php"; 
const char* api   = "tono.tech";  //API Key

int LED_BUILTIN = 2;
uint32_t chipId = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
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

void getdata() {
 
    HTTPClient https;
    https.begin(host);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Chip ID
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    Serial.print("Chip ID: ");
    Serial.println(chipId);


//    String httpRequestData = "api_key=";
//    httpRequestData += api;
//    httpRequestData += "&gpsid=";
//    httpRequestData += chipId;
//    httpRequestData += "&lat=";
//    httpRequestData += "13.831049";
//    httpRequestData += "&lng=";
//    httpRequestData += "100.572044";
//    httpRequestData += "&speed=";
//    httpRequestData += "14.667840";

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN, HIGH);
    
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      String httpRequestData = "api_key=tono.tech&gpsid=2580252&lat=13.831049&lng=100.572044&speed=14.667840";
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
  
  Serial.println();
  Serial.println("Waiting 2min before the next round...");
  
}

void loop() {
  getdata();
  delay(30000);
}
