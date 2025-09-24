
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN D1   // NodeMCU PIN D1
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "o7iyfVst5Z2thnLYd0Zv13A_xDAMrJuE";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SookYenFarm";
char pass[] = "0863741677";

void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk.iot-cm.com", 8080);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
}

void loop()
{
  // Temperature
  dht.begin();
  
  float humidity = dht.readHumidity(); // ดึงค่าความชื้น
  float temperature = dht.readTemperature(); // ดึงค่าอุณหภูมิ

  Serial.print(" Temperature: ");
  Serial.print(temperature);
  Serial.print(" Humidity: ");
  Serial.println(humidity);
  
  Blynk.run();
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  delay(60000);

}
