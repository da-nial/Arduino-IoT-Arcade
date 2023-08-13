const int middlePIN = D1;
const int rightPIN = D2;
const int leftPIN = D3;

boolean onMiddle;
boolean onRight;
boolean onLeft;

#include <Servo.h>
Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(115200);  
  myservo.attach(D4);
  myservo.write(90);
}
void loop() {

  onMiddle = getState(middlePIN, "middle");
  onRight = getState(rightPIN, "right");
  onLeft = getState(leftPIN, "left");

  if (onMiddle){
    myservo.write(90);
    Serial.println("on track");
    }
  else if (onRight){
    myservo.write(0);
    Serial.println("move right");
    }
  else if (onLeft){
    myservo.write(180);
    Serial.println("move left");
    }
  
  delay(1000);
}

bool getState(int pin, String pinTitle){
    int sensorValue = digitalRead(pin);
    Serial.print("Pin ");
    Serial.print(pinTitle);
    Serial.print(" value: ");
    Serial.println(sensorValue);

    return (sensorValue == 1); 
}
