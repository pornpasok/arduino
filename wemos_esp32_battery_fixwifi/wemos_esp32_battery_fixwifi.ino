#//include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <PubSubClient.h>
#include <WiFi.h>
#//include <DNSServer.h>
#//include <WiFiManager.h>



// Update these with values suitable for your network.
const char* ssid = "SookYenFarm";
const char* password = "0863741677";

//const char* ssid = "the4c";
//const char* password = "1122334455";

// Config MQTT Server
#define mqtt_server "m15.cloudmqtt.com"
#define mqtt_port 15443
#define mqtt_user "gcixpcpd"
#define mqtt_password "Je-l1HiaSjjr"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(16, OUTPUT);

  Serial.begin(115200);
  delay(10);

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
  
  

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  } else {

    //client.subscribe("/", String(ESP.getChipId()).c_str());
    client.subscribe("/U0690001b98f8bed8c474fc3409fbeba5");
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  int i = 0;
  while (i < length) msg += (char)payload[i++];
  Serial.println(msg);
  digitalWrite(16, (msg == "on" ? LOW : HIGH));
}
