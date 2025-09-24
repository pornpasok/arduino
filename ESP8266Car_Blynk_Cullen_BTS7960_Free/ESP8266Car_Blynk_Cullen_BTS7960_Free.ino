
/*How to make a wifi controlled car using Nodemcu ESP8266  and NEW Blynk App
  https://www.youtube.com/c/TechieLagan
*/
/*
 Node MCU v3 
 LED_BUILTIN = D4/2 Internal LED for LoLin V3
*/

#define BLYNK_TEMPLATE_ID "TMPL66mC_3v8w"
#define BLYNK_TEMPLATE_NAME "ESPCarFree"
#define BLYNK_AUTH_TOKEN "9PknNMP4vNfIXx69mtevCGLb5gAyfVZi"

// Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define the motor pins
//#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D5
#define IN4 D6
//#define ENB D7
#define LIGHT D0

// Variables for the Blynk widget values
int fwd = 0;
int bwd = 0;
int left = 0;
int right = 0;
int Speed = 100;
int light = 1; 
int realspeed = 100;
int i = 0;

char auth[] = BLYNK_AUTH_TOKEN; //Enter your Blynk auth token
char ssid[] = "SookYenFarm"; //Enter your WIFI name
char pass[] = "0863741677"; //Enter your WIFI passowrd


void setup() {
  Serial.begin(9600);
  //Set the motor pins as output pins
  //pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //pinMode(ENB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHT, OUTPUT);

  // Stop motor
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

// Get the joystick values
BLYNK_WRITE(V0) {
  fwd = param.asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  bwd = param.asInt();
}
// Get the joystick values
BLYNK_WRITE(V2) {
  left = param.asInt();
}
// Get the joystick values
BLYNK_WRITE(V3) {
  right = param.asInt();
}
//Get the slider values
BLYNK_WRITE(V4) {
  Speed = param.asInt();
}
//
////Get the switch values
//BLYNK_WRITE(V5) {
//  light = param.asInt();
//}

// Check these values using the IF condition
void smartcar() {
//  Serial.print("f: ");
//  Serial.print(fwd);
//  Serial.print(" b: ");
//  Serial.print(bwd);
//  Serial.print(" l: ");
//  Serial.print(left);
//  Serial.print(" r: ");
//  Serial.println(right);
//
//  delay(1000);
  
  Serial.println("carForward");
  if (fwd == 1) {
    carForward();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carForward");
  } else if (bwd == 1) {
    carBackward();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carBackward");
  } else if (left == 1) {
    carLeft();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carLeft");
  } else if (right == 1) {
    carRight();
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("carRight");
  } else {
    carStop();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("carstop");
  }
}

void loop() {
  Blynk.run();// Run the blynk function
  smartcar();// Call the main function
  Serial.print("Light: ");
  Serial.println(light);
  digitalWrite(LIGHT, light);
  
}

/**************Motor movement functions*****************/
void carForward() {
  //realspeed = (Speed*y)/100;
  realspeed = Speed;
  Serial.print("realspeed: ");
  Serial.println(realspeed);
  //analogWrite(ENA, Speed);
  //analogWrite(ENB, Speed);
  analogWrite(IN1, realspeed);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, realspeed);
}
void carBackward() {
  //realspeed = (Speed*(100-y))/100;
  realspeed = Speed;
  Serial.print("realspeed: ");
  Serial.println(realspeed);
  //analogWrite(ENA, Speed);
  //analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  analogWrite(IN2, realspeed);
  analogWrite(IN3, realspeed);
  digitalWrite(IN4, LOW);
}
void carLeft() {
  //realspeed = (Speed*(100-x))/100;
  realspeed = Speed;
  Serial.print("realspeed: ");
  Serial.println(realspeed);
  //analogWrite(ENA, Speed);
  //analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  analogWrite(IN2, realspeed);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, realspeed);
}
void carRight() {
  //realspeed = (Speed*x)/100;
  realspeed = Speed;
  Serial.print("realspeed: ");
  Serial.println(realspeed);
  //analogWrite(ENA, Speed);
  //analogWrite(ENB, Speed);
  analogWrite(IN1, realspeed);
  digitalWrite(IN2, LOW);
  analogWrite(IN3, realspeed);
  digitalWrite(IN4, LOW);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
