/**
 * An example showing how to put ESP8266 into Deep-sleep mode
 */
 
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //pinMode(LED_BUILTIN, OUTPUT);
  
  // Wait for serial to initialize.
  while(!Serial) { 
    //digitalWrite(LED_BUILTIN, LOW);
    //delay(10);
  }
  
  Serial.println("I'm awake.");

  Serial.println("Going into deep sleep for 20 seconds");
  ESP.deepSleep(20e6); // 20e6 is 20 microseconds
}

void loop() {
}
