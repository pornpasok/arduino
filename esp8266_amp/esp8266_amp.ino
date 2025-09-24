int analogInPin = A1; // Analog input pin that the carrier board OUT is connected to

int sensorValue = 0; // value read from the carrier board
int outputValue = 0; // output in milliamps

void setup() { // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  
}
void loop() {
  sensorValue = analogRead(analogInPin); // read the analog in value:
  outputValue = (((long)sensorValue * 5000 / 1024) - 512 ) * 1000 / 185; // convert to milli amps
  Serial.print("sensor = " );  // print the results to the serial monitor:
  Serial.print(sensorValue);
  Serial.print("\t Current (ma) = ");
  Serial.println(outputValue);
  delay(500); // wait 10 milliseconds before the next loop
}

