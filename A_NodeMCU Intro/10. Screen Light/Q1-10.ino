const int analogInPin = A0;
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  analogWrite(D1, 1023);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
  analogWrite(D1, 1024 - sensorValue);

  delay(100);
}
