#include <SoftwareSerial.h>
#include "PMS.h"

SoftwareSerial serialDust(12, 14); // RX, TX

PMS pms(serialDust);
PMS::DATA data;

float temperature = 0;
unsigned int humandity = 0;

float set_temp_offset = 4.8;; 
float set_rhum_offset = 7.0;

void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  serialDust.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  pinMode(LED_BUILTIN, OUTPUT);
  pms.passiveMode();    // Switch to passive mode
  
}

void loop()
{
  Serial.println("Waking up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  //delay(30000);

  int i;
  int delay_time;
  
  for (i=1; i<=30; i++) {
      Serial.println(i);
 
      if (i>=27) {
        int j;
        for (j=1; j<=5; j++) {
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("LED OFF");
          delay(100);
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("LED ON");
          delay(100);   
        }
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED OFF");
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED ON");
        delay(500);   

      }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED OFF");
  

  Serial.println("Send read request...");
  pms.requestRead();

  Serial.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    temperature = data.AMB_TMP/10 + set_temp_offset;
    humandity = data.AMB_HUM/10 + set_rhum_offset;
    
    Serial.print("Temperature (C): ");
    Serial.println(temperature);

    Serial.print("Humidity (%): ");
    Serial.println(humandity);
  }
  else
  {
    Serial.println("No data.");
  }

  Serial.println("Going to sleep for 60 seconds.");
  pms.sleep();
  delay(60000);
}
