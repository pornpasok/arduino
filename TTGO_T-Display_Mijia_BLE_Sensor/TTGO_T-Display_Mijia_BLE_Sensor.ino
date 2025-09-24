#include <WiFi.h>
#include <HTTPClient.h>

#include <WiFiManager.h>

// Weather Server 
const char* serverName = "http://tonofarm.herokuapp.com/esp-post-data.php";
String apiKeyValue = "tonofarm.io";
String sensorLocation = "12.7581423,102.1468503";

uint32_t chipId = 0;

float Temp;
float Humidity;
float Pressure;
float vbatt;
float batt_percent;

#include <BLEDevice.h>
#include <SimpleTimer.h>

#define uS_TO_S_FACTOR 1000000     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  59        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */

#define LYWSD03MMC_ADDR "a4:c1:38:1d:88:2e"

#define LED_BUILTIN 2

BLEClient* pClient;

static BLEAddress htSensorAddress(LYWSD03MMC_ADDR);

bool connectionSuccessful = false;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      Serial.println("Connected");
    }

    void onDisconnect(BLEClient* pclient) {
      Serial.println("Disconnected");
      if (!connectionSuccessful) {
        Serial.println("RESTART");
        ESP.restart();
      }
    }
};

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  
  Temp = (pData[0] | (pData[1] << 8)) * 0.01; //little endian 
  Humidity = pData[2];
  vbatt = ((pData[4] * 256) + pData[3]) / 1000.0;
  batt_percent = (vbatt - 2.1) * 100.0;
  
  Serial.printf("Temperature = %.1f : Humidity = %.1f : VBatt = %.1f : Battery_percent = %.1f \n", Temp, Humidity, vbatt, batt_percent);
  pClient->disconnect();

  Serial.println("End.");
}

void registerNotification() {

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our characteristic");
  pRemoteCharacteristic->registerForNotify(notifyCallback);
}

void connect() {
  // ChipID
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  
  Serial.print("ESP ChipID: ");
  Serial.println(chipId);

  // Wi-Fi Signal
  long rssi;
  rssi = WiFi.RSSI();
  Serial.print("WiFi Signal: ");
  Serial.println(rssi);
  
  // Use WiFiClient class to create TCP connections
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + chipId
                            + "&location=" + sensorLocation + "&value1=" + Temp
                            + "&value2=" + Humidity + "&value3=" + Pressure + "&value4=" + rssi +  "&value5=" + vbatt + "";
      
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
  Serial.println("Starting MJ client...");
  delay(500);

  BLEDevice::init("ESP32");
  createBleClientWithCallbacks();
  delay(500);
  connectSensor();
  registerNotification();

  // WiFiManager
  //WiFiManager wifiManager;
  //wifiManager.autoConnect("AutoConnectAP");

  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.setTimeout(2000);
  // Wait for serial to initialize.
  while (!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running Deep Sleep Firmware!");
  Serial.println("-------------------------------------");

  //connect();

  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");

  // Configure the wake up source as timer wake up  
  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  // ESP Deep Sleep Mode
  //esp_deep_sleep_start();
}

void loop() {
  // do nothing
}

void createBleClientWithCallbacks() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor() {
  pClient->connect(htSensorAddress);
  connectionSuccessful = true;
}
