void setup() {
Serial.begin(9600);
}
void loop() {
float average = 0;
for(int i = 0; i < 1000; i++) {
average = average + (.0264 * analogRead(A0) -13.52) / 1000;
//average = average + (.0264 * analogRead(A0) -13.51) / 1000;
delay(1);
}
Serial.println(average);
}
