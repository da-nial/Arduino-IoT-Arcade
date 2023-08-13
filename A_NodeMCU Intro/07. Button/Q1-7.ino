const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;
boolean state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

}

void loop() {
  // read the analog in value
  sensorValue = analogRead(analogInPin);
 
  // print the readings in the Serial Monitor
  Serial.println(sensorValue);
  if (sensorValue > 900){
      Serial.print("changing state from ");
      Serial.print(state);
      Serial.print(" to ");
      Serial.println(!state);
      state = !state;   
      
    }
  
  if (state == true){
      digitalWrite(D0, HIGH);
    }
  else{
    digitalWrite(D0, LOW);
    }
  
  delay(1000);

}
