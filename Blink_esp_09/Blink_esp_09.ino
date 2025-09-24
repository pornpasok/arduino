/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  pinMode(D1,INPUT);
  digitalWrite(D4,LOW);
}

// the loop function runs over and over again forever
void loop() {
  /*
  digitalWrite(D4, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(D4, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000); 
  */ 
  // wait for a second
  Serial.println(digitalRead(D1));
  if(digitalRead(D1)==HIGH) {
    digitalWrite(D4, LOW);
    Serial.println("Movement detected.");
  }
  else {
    digitalWrite(D4, HIGH);
    Serial.println("Nothing.");
  }
    
  delay(1000);

  /*
    if(digitalRead(D0)==HIGH)  
    {
      Serial.println("Movement detected.");
    }
    else  
    {
      Serial.println("Nothing.");
    }
    delay(1000);

 */
 
}
