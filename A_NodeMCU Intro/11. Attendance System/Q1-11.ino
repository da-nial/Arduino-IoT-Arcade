#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

#define LED D0
#define BUZZER D1
#define SERVO D2

Servo myservo;  // create servo object to control a servo

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;
long classStart;
boolean classOngoing = false;
long classDuration = 15000;
long validEntrance = 10000;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;
boolean AmirhosseinPresent = false;
boolean DanialPresent = false;

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO, OUTPUT);

  myservo.attach(BUZZER);
  myservo.write(0);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  if (sensorValue > 900){
      // class begins
      if (classOngoing){
        Serial.println("class is already holding");
        }
      else {
        classStart = millis();
        classOngoing = true;
        AmirhosseinPresent = false;
        DanialPresent = false;
        Serial.println("class starts");
        }
     delay(2000);
    }

  if (!classOngoing) {
    return;
  }

  String username;
  if ( rfid.PICC_IsNewCardPresent()){
        if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    username = getNameByTag(tag);
    Serial.println("User " + username + " entered their card");
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  if (username != "invalid"){
    if ((millis() - classStart) <= validEntrance){
      markPresent(username);
      }  
    else {
      Serial.println("user tried to enter in invalid time");
      pulse(BUZZER, 1000);
      }
    } 
  }

  
  if ((millis() - classStart) >= classDuration){
    classOngoing = false;
    Serial.println("end of class");

    // presence absence
    displayPresenceAbsence();
    }
}


String getNameByTag(String tag){
  if (tag == "130610027") {
    return "Amirhossein";
  }
  else if (tag == "1631422827"){
    return "Danial";
  }
  else {
    return "invalid";
    }
}

void markPresent(String user_name){
  if (user_name == "Amirhossein") {
    AmirhosseinPresent = true;
    Serial.println("Amirhossein was marked as present");
  }
  else if (user_name == "Danial"){
    DanialPresent = true;
    Serial.println("Danial was marked as present");
  }
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, HIGH);
  myservo.write(90);
  delay(3000);
  digitalWrite(LED, LOW);
  myservo.write(0);
}

void displayPresenceAbsence(){
  if (AmirhosseinPresent){
   Serial.println("Amirhossein was present");
   }
  else {
    Serial.println("Amirhossein was absent");
    }
    if (DanialPresent){
   Serial.println("Danial was present");
   }
  else {
    Serial.println("Danial was absent");
    }
    // Sorry for filthy code :/
}

void pulse(int pin, long duration){
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}
  
