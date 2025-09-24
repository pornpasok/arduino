#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "DHT.h"

//define SECONDS_DS(seconds) ((seconds)*1000000UL)
#define DHTPIN D1   // NodeMCU PIN D1
//#define DHTTYPE DHT11     //DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//dataSource = "TandHFeedonFB"

const char* ssid     = "SookYenFarm";
const char* password = "0863741677";

#define APPID   "SookYenFarm"
#define KEY     "HX9v0a6j0cxTRa2"
#define SECRET  "C5PcUGus3HvPTsgjtPgaWojF1"
#define ALIAS   "nodemcu"

WiFiClient client;

int led = D4;

int timer = 0;
MicroGear microgear(client);
/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
    String msg2 = String((char*)msg);

    Serial.println(msg2);

    if (msg2 == "ON") {
      //digitalWrite(relayPin, HIGH);
      digitalWrite(led, LOW);
      microgear.chat("testledstatus", "ON");    // led status
    }
    else if (msg2 == "OFF") {
     // digitalWrite(relayPin, LOW);
      digitalWrite(led, HIGH);
      microgear.chat("testledstatus", "OFF");  // led status
    }
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}
/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}


void setup() {

    pinMode(led, OUTPUT);
    
    dht.begin();
    
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(PRESENT,onFoundgear);
    microgear.on(ABSENT,onLostgear);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
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

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() {
    if (microgear.connected()) {
        Serial.println("connected");
        microgear.loop();
    
        if (timer >= 1000) {
            Serial.println("Publish...");
            microgear.chat(ALIAS,"Hello ^^");

            //******  read DHT sensor very 2sec
            delay(2000);
            float h = 0.00f;
            float t = 0.00f;
            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            t = dht.readTemperature();
  
            if (isnan(h) || isnan(t)) {
              Serial.println("Failed to read from DHT sensor!");
              //return;
            }
            else {
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t");
              Serial.print("Temperature: ");
              Serial.print(t);
              Serial.println(" *C ");
  
              /* Chat with the microgear named ALIAS which is myself */
              microgear.chat("nodemcu/temp", (String)t);
              microgear.chat("nodemcu/humid", (String)h);
  
              char topic_temp[MAXTOPICSIZE];
              char topic_humid[MAXTOPICSIZE];
              sprintf(topic_temp, "/gearname/%s/temp", ALIAS);
              sprintf(topic_humid, "/gearname/%s/humid", ALIAS);
              //retain message
              microgear.publish(topic_temp, String(t), true);
              microgear.publish(topic_humid, String(h), true);
            }
            
            timer = 0;
        } 
        else timer += 100;        
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(1000);
}
