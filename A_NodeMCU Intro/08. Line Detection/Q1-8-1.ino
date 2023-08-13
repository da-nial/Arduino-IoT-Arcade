const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(analogInPin);
  Serial.print("Sensor value: ");
  Serial.print(sensorValue);
  Serial.print("====> mapped value: ");
  Serial.println(map(sensorValue, 0, 1023, 0, 100));

  delay(3000);

}
