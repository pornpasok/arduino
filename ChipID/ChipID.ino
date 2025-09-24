uint64_t chipid;  

void setup() {
  Serial.begin(115200);
}
char ssid [15];
void loop() {
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32_%08X\n",(uint32_t)chipid);

  snprintf(ssid,15,"ESP32_%08X\n",(uint32_t)chipid);
  
  //Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  //Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  
  Serial.print("Chip ID: ");
  Serial.println(ssid);

  delay(3000);

}
