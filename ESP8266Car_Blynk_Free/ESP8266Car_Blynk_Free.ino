
/*How to make a wifi controlled car using Nodemcu ESP8266  and NEW Blynk App
  https://www.youtube.com/c/TechieLagan
*/

#define BLYNK_TEMPLATE_ID "TMPL66mC_3v8w"
#define BLYNK_TEMPLATE_NAME "ESPCarFree"
#define BLYNK_AUTH_TOKEN "9PknNMP4vNfIXx69mtevCGLb5gAyfVZi" 

// Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define the motor pins
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

// Variables for the Blynk widget values
int f = 0;
int b = 0;
int l = 0;
int r = 0;

int Speed = 100;

char auth[] = BLYNK_AUTH_TOKEN; //Enter your Blynk auth token
char ssid[] = "SookYenFarm"; //Enter your WIFI name
char pass[] = "0863741677"; //Enter your WIFI passowrd


void setup() {
  Serial.begin(9600);
  //Set the motor pins as output pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

// Get the joystick values
BLYNK_WRITE(V0) {
  f = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  b = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V2) {
  l = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V3) {
  r = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V4) {
  Speed = param.asInt();
}

// Check these values using the IF condition
void smartcar() {
  if (f == 1) {
    carForward();
    //digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carForward");
  } else if (b == 1) {
    carBackward();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carBackward");
  } else if (l == 1) {
    carLeft();
    //digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carLeft");
  } else if (r == 1) {
    carRight();
    //digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carRight");
  } else {
    carStop();
    //digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("carstop");
  }
}
void loop() {
  Blynk.run();// Run the blynk function
  smartcar();// Call the main function
}

/**************Motor movement functions*****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
