#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SookYenFarm";
const char* password = "0863741677";

WebServer server(80);

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

int motor2Pin1 = 33; 
int motor2Pin2 = 25; 
int enable2Pin = 32; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
 }

void handleCar() {
 String message = "";
 int BtnValue = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="a")
    {
      String s = server.arg(i);
      BtnValue = s.toInt();
    }
    Serial.println(server.argName(i) + ": " + server.arg(i) + "\n");
  }

  ledcWrite(pwmChannel, dutyCycle);
  
  switch (BtnValue) {
   case 1: // sola donuş
      // Move the DC motor forward at maximum speed
      Serial.println("Moving Forward");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW); 
      break;
   case 2: // düz ileri
       // Move the DC motor forward at maximum speed
      Serial.println("Moving Forward");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW); 
     break;
  case  3:// saga donuş  
      // Move the DC motor forward at maximum speed
      Serial.println("Moving Forward");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW); 
     break;
  case  4:// tam sola donuş   
      // Move DC motor Left at maximum speed
      Serial.println("Moving Backwards");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH); 
     break;
  case 5: // stop   
    // Move the DC motor forward at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW); 
   break;     
  case  6://   
    // Move DC motor Right at maximum speed
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
    break;
  case 7://sol geri
     // Move the DC motor forward at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
    break;
  case 8:// tam geri
    // Move DC motor backwards at maximum speed
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 
    break;    
  case 9:// sag geri  
     // Move the DC motor forward at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
    default:
    break;
  }
  
 
  message += "<html> <head> <title>Gungor yalcin</title><head>";
  message += "<body><h3>Wifi Robot Car NodeMCU  Web Server</h1>";
  message += "<table> "; 
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=1\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "<td><p><a href=\"/car?a=2\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">^</button></a></p> ";
  message += "<td><p><a href=\"/car?a=3\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=4\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> < </button></a></p> ";
  message += "<td><p><a href=\"/car?a=5\"><button style=\"width:100;height:100;font-size:40px;\" class=\"button\">Stop</button></a></p> ";
  message += "<td><p><a href=\"/car?a=6\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\"> > </button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=7\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">/</button></a></p> ";
  message += "<td><p><a href=\"/car?a=8\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">v</button></a></p> ";
  message += "<td><p><a href=\"/car?a=9\"><button style=\"width:100;height:100;font-size:100px;\" class=\"button\">\\</button></a></p> ";
  message += "</table> "; 
  message += "</body></html>";
  server.send(200, "text/html", message);
 }




void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 

void setup() {
 
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);
  
  Serial.begin(115200); 
 
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/car", handleCar);

server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
