#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"


#define ADC_EN          14  //ADC_EN is the ADC detection enable port
#define ADC_PIN         34

char buff[512];
int vref = 1100;
int btnCick = false;


void showVoltage()
{
    static uint64_t timeStamp = 0;
    if (millis() - timeStamp > 1000) {
        timeStamp = millis();
        uint16_t v = analogRead(ADC_PIN);
        float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
        String voltage = "Voltage :" + String(battery_voltage) + "V";
        Serial.println(voltage);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    pinMode(ADC_EN, OUTPUT);
    digitalWrite(ADC_EN, HIGH);

  
}

void loop()
{
   
  showVoltage();
    
}
