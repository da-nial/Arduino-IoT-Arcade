#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const int trigPin = D0; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D1; // Echo Pin of Ultrasonic Sensor

long duration, distance;

bool isPending = false;
long pendingStart;

void setup() {
  Serial.begin(115200);
  myservo.attach(D2);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  pinMode(trigPin, OUTPUT); // Trigger
  pinMode(echoPin, INPUT); // Echo
}

void loop() {
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);

   duration = pulseIn(echoPin, HIGH);
   distance = (duration * 0.034) / 2;
   Serial.print("Distance: ");
   Serial.println(distance);
   delay(500);

   if (distance < 5) {
    if (!isPending) {
      Serial.println("Start Pending");
      isPending = true;
      pendingStart = millis();
    }
    else if (millis() - pendingStart > 3000) {
      Serial.print("Has been pending for 3 seconds, Can will be open for 4 seconds");
      isPending = false;
      myservo.write(90);
      delay(4000);
      myservo.write(0);
    }
   }
}
