int analogInPin = A1; // Analog input pin that the carrier board OUT is connected to
int sensorValue = 0; // value read from the carrier board
int outputValue = 0; // output in milliamps

void setup() { // initialize serial communications at 9600 bps:
Serial.begin(9600); }

void loop() {
// read the analog in value:
sensorValue = analogRead(analogInPin);

// convert to milli amps
outputValue = (((long)sensorValue * 5000 / 1024) - 500 ) * 1000 / 133;

/* sensor outputs about 100 at rest. Analog read produces a value of 0-1023, equating to 0v to 5v. "((long)sensorValue * 5000 / 1024)" is the voltage on the sensor's output in millivolts. There's a 500mv offset to subtract. The unit produces 133mv per amp of current.*/

// print the results to the serial monitor:
Serial.print("sensor = " );
Serial.print(sensorValue);
Serial.print("\t Current (ma) = ");
Serial.println(outputValue);

// wait 10 milliseconds before the next loop
// for the analog-to-digital converter to settle // after the last reading:
delay(500); }


