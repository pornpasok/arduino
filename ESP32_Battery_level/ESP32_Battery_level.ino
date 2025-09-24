float voltage;
int bat_percentage;
int analogInPin  = 34;    // Analog input pin
int sensorValue;
float calibration = 0.40; // Check Battery voltage using multimeter & add/subtract the value

void setup()
{
  Serial.begin(115200);
  delay(100);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiply by two as voltage divider network is 100K & 100K Resistor
  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage
  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  
 
  
    //Print data on serial monitor
  Serial.print("Analog Value = ");
  Serial.println(sensorValue);
  Serial.print("Output Voltage = ");
  Serial.println(voltage);
  Serial.print("Battery Percentage = ");
  Serial.println(bat_percentage);
  Serial.println();
  Serial.println("****************************");
  Serial.println();
  delay(1000);
 
       if (bat_percentage <=30)
    {
      Serial.println("Battery level below 30%, Charge battery on time");
      delay(500);
    }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
