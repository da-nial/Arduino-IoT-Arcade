#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);

#include <Servo.h>
Servo myservo;

const int trigPin = D3; // Trigger Pin of Ultrasonic Sensor
const int echoPin = D2; // Echo Pin of Ultrasonic Sensor

const int buzzPin = D0;

long duration, distance;

long lastOpened;

long AUTO_CLOSE_TIMEOUT = 20000;

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  setupAccessPoint();
  setupServo();
  setupUltraSonic();
  setupBuzzer();
  setupLED();

  lastOpened = -1;

  setupWebServer();
}

void loop() {
  autoClose();
}

String processor(const String& var) {
  return String();
}

void setupAccessPoint(){
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void setupServo() {
  pinMode(D4, OUTPUT);
  myservo.attach(D4);
  myservo.write(0);
}

void setupLED() {
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
}

void setupUltraSonic() {
  pinMode(trigPin, OUTPUT); // Trigger
  pinMode(echoPin, INPUT); // Echo
}

void setupBuzzer() {
  pinMode(buzzPin, OUTPUT);
//  digitalWrite(buzzPin, LOW);
}

void setupWebServer() {
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", webpage, processor);
  });

  server.on("/open", HTTP_GET, [] (AsyncWebServerRequest *request) {
    openDoor();
    request->send_P(200, "text/html", webpage, processor);
  });

  server.on("/close", HTTP_GET, [] (AsyncWebServerRequest *request) {
    bool didClose = closeDoor();
    if (didClose) {
      request->send_P(200, "text/html", webpage, processor);
    }
    else {
      request->send_P(200, "text/html", "Obstacle Detected! Could not close the door.");
    }
  });

  server.begin();
}

void openDoor() {
  Serial.println("Door is opening");
  digitalWrite(D1, HIGH);
  
  myservo.write(180);
  delay(100);
  
  digitalWrite(D1, LOW);

  lastOpened = millis();
  Serial.println("Opened!");
}

bool closeDoor() {
  Serial.println("Door is closing");
  digitalWrite(D1, HIGH);
  
  if (hasNearObstacles()) {
    digitalWrite(D1, LOW);
  
    buzz();
    return false;
  }
  
  // Turn Servo to 0
  for (int pos = 180; pos >= 0; pos -= 5) {
    if (hasNearObstacles()) {
      openDoor();
      digitalWrite(D1, LOW);
      buzz();
      
      return false;
    }
    Serial.print("current pos: ");
    Serial.println(pos);
    myservo.write(pos);              
    delay(5);
  }

  lastOpened = -1;

  Serial.println("Closed!");
  digitalWrite(D1, LOW);
  return true;
}

bool hasNearObstacles() {
  int distance = getDistance();
  if (distance < 5) {
    Serial.print("Obstacle Detected in ");
    Serial.println(distance);
    return true;
  }
  return false;
}

int getDistance() {
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);

   duration = pulseIn(echoPin, HIGH);
   distance = (duration * 0.034) / 2;
   return distance;
}

void buzz() {
  tone(buzzPin, 1500, 1000); delay(150);
}

void autoClose() {
  if (lastOpened == -1) {
    return;
  }
  
  if (millis() - lastOpened > AUTO_CLOSE_TIMEOUT){
    Serial.println("Automatic Close Triggered!");
    closeDoor();
  }
}
