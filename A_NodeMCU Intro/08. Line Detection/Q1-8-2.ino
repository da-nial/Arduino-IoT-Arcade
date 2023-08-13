const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
}
void loop() {
  // read the analog in value
  sensorValue = analogRead(analogInPin);
 
  // print the readings in the Serial Monitor
  Serial.println(sensorValue);
  if (sensorValue > 900){  
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
    }
  else {
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
    }
  delay(1000);
}
