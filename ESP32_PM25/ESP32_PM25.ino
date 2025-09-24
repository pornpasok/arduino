/* Connect to SHARP PM2.5 Sensor*/
#include <PMsensor.h>
PMsensor PM;
/* connect to wifi */
#include <WiFi.h>
//const char* ssid     = "TonMiA2"; // ssid wifi 
//const char* password = "0863741677"; // it is recommended to use WPA/WPA2

const char* ssid     = "the4c"; // ssid wifi 
const char* password = "1122334455"; // it is recommended to use WPA/WPA2

WiFiClient client;
/* connect to netpie */
#include <MicroGear.h>
#define APPID "ESP32PM25" // APPLICATION (APPID) 
#define KEY "bsOzVnSgnbRnk8K" // device key 
#define SECRET "2NgE9WfjLNxrseRofEFWq8PEu"  // *secret device key
#define FEEDID  "ESP32PM25"  // feed name
#define ALIAS   "esp32pm25"
MicroGear microgear(client);
/* Variable */
int timer = 0;
char str[32];

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() {
  Serial.begin(115200);
  // Initialize PM Sensor
  /////(infrared LED pin, sensor pin)  /////
  PM.init(16, 36);
  // Initialize microgear
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);     //optional
  microgear.on(ERROR, onConnected);
  microgear.on(INFO, onConnected);
  //Wait for WiFi Establishement
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
}

void loop() {
  if (microgear.connected()) {
    //Serial.print(".");
    microgear.loop();
    if (timer >= 60000) {

      float pm = 0;
      int err = PMsensorErrSuccess;
  
      if ((err = PM.read(&pm, true, 0.1)) != PMsensorErrSuccess) {
        Serial.print("data Error = ");
        Serial.println(err);
        return;
      }
      Serial.print("PM2.5: ");
      Serial.print(pm);
      Serial.println(" ppm");

      //STRING FOR FEED
      String  data = "{\"Room_PM2_5\":"; //FEED NAME HERE
      data += pm ;
      data += "}";

      sprintf(str, "%.2f",pm);
      Serial.println(str);
      microgear.publish("/roomquality", str); //Publish to the NETPIE for Freeboard and the other utilization
      Serial.println((char*) data.c_str());
      Serial.print("Sending --> ");
      microgear.writeFeed(FEEDID, data);    //YOUR  FEED ID, API KEY
      
      timer = 0;
    }
    else timer += 200;
  } else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 200;
  }
    delay(200);
}
