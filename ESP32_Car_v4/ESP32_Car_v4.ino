/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

int motor2Pin1 = 33; 
int motor2Pin2 = 25; 
int enable2Pin = 32; 

int LED_BUILTIN = 2;

// HC-SR04
const int pingPin = 23;
int inPin = 22;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;


long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
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

  // testing
  Serial.print("Testing DC Motor...");
}


  // Move the car forward
  void moveForward()
  {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.println("car is moving forward...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  }

  // Move the car backward
  void moveBackward()
  {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.println("car is moving backward...");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  }
  
// Turn the car left
  void turnLeft()
  {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.println("car is turning left...");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  }

  // Turn the car right
  void turnRight()
  {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.println("car is turning right...");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);    
  }

  // Stop the car
  void stop()
  {
    Serial.println("car is stopping...");
    ledcWrite(pwmChannel, 0);

    // Turn off motors
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }


void loop() {

  // Duration cm
  long duration, cm;
 
  pinMode(pingPin, OUTPUT);
   
   
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);
   
  cm = microsecondsToCentimeters(duration);
   
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);


//  moveForward();
//  delay(2000);
//
//  moveBackward();
//  delay(2000);
//
//  turnRight();
//  delay(1000);
//
//  turnLeft();
//  delay(1000);
//
//  stop();
//  delay(5000);
  
  if (cm > 20) {
    moveForward();
    delay(600);
  }
  else if (cm <= 20) {
    moveBackward();
    delay(600);
    turnRight();
    //turnRight();
    delay(600);
  }
  else {
    stop();
    delay(1000);
    moveBackward();
    delay(600);
    turnLeft();
    delay(800);
    stop();
    delay(1000);
  }
    
//  ledcWrite(pwmChannel, dutyCycle);
//  
//  // Move the DC motor forward at maximum speed
//  Serial.println("Moving Forward");
//  digitalWrite(motor1Pin1, HIGH);
//  digitalWrite(motor1Pin2, LOW); 
//  digitalWrite(motor2Pin1, HIGH);
//  digitalWrite(motor2Pin2, LOW); 
//  delay(2000);
//
//  // Stop the DC motor
//  Serial.println("Motor stopped");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, LOW);
//  digitalWrite(motor2Pin1, LOW);
//  digitalWrite(motor2Pin2, LOW);
//  delay(1000);
//
//  // Move DC motor backwards at maximum speed
//  Serial.println("Moving Backwards");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, HIGH); 
//  digitalWrite(motor2Pin1, LOW);
//  digitalWrite(motor2Pin2, HIGH); 
//  delay(2000);
//
//  // Stop the DC motor
//  Serial.println("Motor stopped");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, LOW);
//  digitalWrite(motor2Pin1, LOW);
//  digitalWrite(motor2Pin2, LOW);
//  delay(1000);

  // Move DC motor forward with increasing speed
//  digitalWrite(motor1Pin1, HIGH);
//  digitalWrite(motor1Pin2, LOW);
//  digitalWrite(motor2Pin1, HIGH);
//  digitalWrite(motor2Pin2, LOW);
//  while (dutyCycle <= 255){
//    ledcWrite(pwmChannel, dutyCycle);   
//    Serial.print("Forward with duty cycle: ");
//    Serial.println(dutyCycle);
//    dutyCycle = dutyCycle + 5;
//    delay(500);
//  }
//  dutyCycle = 200;
}
