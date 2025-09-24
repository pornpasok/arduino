#define uS_TO_S_FACTOR 1000000     /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  595        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */

// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "Internet"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = "true"; // GPRS User
const char gprsPass[] = "true"; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = ""; 

// Server details
const char server[] = "tonofarm.herokuapp.com";
const char resource[] = "/esp-post-data.php";
String api = "tonofarm.io";  //API Key
String sensorLocation = "12.7581423,102.1468503";

int LED_BUILTIN = 13;

float t;
float h;
float p;
uint32_t chipId = 0;
float batteryLevel = analogRead(35)*2/1135.0f;

byte keepOnerror;

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22
// BME280 pins
#define I2C_SDA_2            18
#define I2C_SCL_2            19

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#include <Wire.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// I2C for BME280 sensor
TwoWire I2CBME = TwoWire(1);
Adafruit_BME280 bme; 

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  
  keepOnerror = I2CPower.endTransmission();
  Serial.print("I2C KeepOn Code:");
  Serial.println(keepOnerror);
  
  return I2CPower.endTransmission() == 0;
}

void setup() {
  // Set serial monitor debugging window baud rate to 115200
  SerialMon.begin(115200);

  // HTTP POST LED Status
  pinMode(LED_BUILTIN, OUTPUT); // D4

  // Start I2C communication
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  I2CBME.begin(I2C_SDA_2, I2C_SCL_2, 400000);

  // Keep power when running from battery
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
  
  // You might need to change the BME280 I2C address, in our case it's 0x76
  if (!bme.begin(0x76, &I2CBME)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");

    // Random BME280 Sensor Value for Testing
    t = random(20, 30);
    h = random(50, 100);
    p = random(1000, 1100);
  }
  else {
    Serial.println("Find BME280 sensor!");
    // BME280 Sensor
    t = bme.readTemperature();
    h = bme.readHumidity();
    p = bme.readPressure() / 100.0F;

    Serial.print("Temperature(C): ");
    Serial.println(t);
    Serial.print("Humidity(%RH): ");
    Serial.println(h);
    Serial.print("Pressure(hPa): ");
    Serial.println(p);
    Serial.println("--------------------");
  }

  // Configure the wake up source as timer wake up  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

void loop() {
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  }
  else {
    SerialMon.println(" OK");

    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client.connect(server, 80)) {
      SerialMon.println(" fail");
    }
    else {
      SerialMon.println(" OK");

      // Show Connect Status
      Serial.println("LED ON");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      //delay(1000);
      
      // ChipID
      for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
      }

      // Chip ID
      Serial.print("Chip ID: ");
      Serial.println(chipId);

      // Battery Level
      Serial.print("Battery Level ");
      Serial.println(batteryLevel);
      
      // Show Modem Signal
      int csq = modem.getSignalQuality();
      SerialMon.print("Modem Signal: ");
      SerialMon.println(csq);
      
      int csqsignal = map(csq, 0, 31, 0, 100);
      Serial.print("Modem Signal: ");
      Serial.print(csqsignal);
      Serial.println(" %");
  
      // Making an HTTP POST request
      SerialMon.println("Performing HTTP POST request...");
      
      // Show Connect Status
      Serial.println("LED ON");
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      //delay(1000);

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
      httpRequestData += p;
      httpRequestData += "&value4=";  
      httpRequestData += csqsignal;
      httpRequestData += "&value5=";  
      httpRequestData += batteryLevel;
      
      //String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
      //                      + "&location=" + sensorLocation + "&value1=" + String(bme.readTemperature())
      //                      + "&value2=" + String(bme.readHumidity()) + "&value3=" + String(bme.readPressure()/100.0F) + "&value4=" + v + "";
      
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
                             
      client.print(String("POST ") + resource + " HTTP/1.1\r\n");
      client.print(String("Host: ") + server + "\r\n");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
//        while (client.available()) {
//          char c = client.read();
//          SerialMon.print(c);
//          timeout = millis();
//        }
      }
      SerialMon.println();
    
      // Close client and disconnect
      client.stop();

      Serial.println("LED OFF");
      digitalWrite(LED_BUILTIN, LOW);
      //delay(1000);
  
      SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
  // Put ESP32 into deep sleep mode (with timer wake up)
  Serial.print("Sleeping ");
  Serial.print(TIME_TO_SLEEP);
  Serial.println(" seconds ..");
  esp_deep_sleep_start();
}
