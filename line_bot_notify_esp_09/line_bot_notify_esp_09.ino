#include <ESP8266WiFi.h>

const char* ssid = "TonMiA1";
const char* password = "0863741677";
const char* host = "doctor.eggdigital.com";
//https://doctor.eggdigital.com/pushmsg-robot.php?cid=Cbda0de00773c3e8876a566530b3fe3aa&msg=มีผู้บุกรุก

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D4, OUTPUT);
  pinMode(D1,INPUT);
  digitalWrite(D4,LOW);
  
  Serial.begin(9600);

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
}

void gp_fetch_url(char* gp_url) {
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
  String url = gp_url;
  //String url = "/gp/gpControl/status"; 
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" + 
  "Connection: close\r\n\r\n");
  delay(10);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
  Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  // LED Blink
  digitalWrite(BUILTIN_LED, LOW);
  delay(500);
}


// the loop function runs over and over again forever
void loop() {
  // wait for a second
  Serial.println(digitalRead(D1));
  if(digitalRead(D1)==HIGH) {
    // Call Line Bot API
    gp_fetch_url("/pushmsg-robot.php?cid=Cf29847a5bb62b13f83d1c6954fa947e8&msg=ตรวจพบผู้บุกรุก");
    
    digitalWrite(D4, LOW);
    Serial.println("Movement detected.");
  }
  else {
    digitalWrite(D4, HIGH);
    Serial.println("Nothing.");
  }
    
  delay(5000);
 
}
