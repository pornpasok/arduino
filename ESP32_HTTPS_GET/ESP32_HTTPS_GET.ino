#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "SookYenFarm";
const char* password = "0863741677";
const char *host = "https://gpstracking.tono.tech/post-data.php"; 

int LED_BUILTIN = 2;

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
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    // set secure client with certificate
    client->setInsecure();
    //create an HTTPClient instance
    HTTPClient https;

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN, HIGH);
    
    if (https.begin(*client, host)) {  // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
       Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
      delay(1000);
      Serial.println("LED OFF");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  Serial.println();
  Serial.println("Waiting 2min before the next round...");
  
}

void loop() {
  getdata();
  delay(60000);
}
