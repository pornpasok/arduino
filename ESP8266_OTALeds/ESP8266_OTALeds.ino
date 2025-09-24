#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "SookYenFarm"
#define STAPSK  "0863741677"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "OTA-LEDS";

int led_pin = 2;

void setup() {
  Serial.begin(115200);

  /* switch on led */
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Retrying connection...");
  }
  /* switch off led */
  digitalWrite(led_pin, HIGH);

  /* configure dimmers, and OTA server events */

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    digitalWrite(led_pin, LOW);
    delay(1000);
    digitalWrite(led_pin, HIGH);
    delay(1000);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    digitalWrite(led_pin, HIGH);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");

}

void loop() {
  ArduinoOTA.handle();
}
