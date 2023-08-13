#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

String tag;

int userBalance = 20000;
String userCart;

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

Servo myservo;

const int LEDPin = D2;
const int buzzPin = D0;
const int servoPin = D1;

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient * globalClient = NULL;

void setup() {
  Serial.begin(115200);
  delay(3000);
  setupBuzzer();
  setupLED();
  setupRFID();
  setupServo();
  setupAccessPoint();
  setupWebsocket();
  setupWebServer();
}

void loop() {
  ws.cleanupClients();
  if (rfid.PICC_IsNewCardPresent()) {
    finalizeOrder();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.println("Websocket client connection received");
      globalClient = client;
      break;
    case WS_EVT_DISCONNECT:
      Serial.println("Websocket client connection finished");
      globalClient = NULL;
      break;
    default:
      Serial.println("Other Cases");
      break;
  }
}

String processor(const String& var) {
  return String();
}

void setupLED() {
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
}

void setupBuzzer() {
  pinMode(buzzPin, OUTPUT);
}

void setupServo() {
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(0);
}

void setupRFID() {
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void setupAccessPoint(){
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void setupWebsocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setupWebServer() {
  server.on("/yaghoob", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Served Yaghoob Page");
    request->send_P(200, "text/html", webpage, processor);
  });

  server.on("/order", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("order");
    if (request->hasParam("product")) {
        String product = request->getParam("product")->value();
        Serial.println("product: " + product);
        order(product);
    }
    request->send_P(200, "text/html", webpage, processor);
  });

  server.begin();
}

void order(String product) {
  userCart = product;
  Serial.println("Enter your card for product " + product);
}

void finalizeOrder() {
  if (userCart == "") {
    return;
  }
  
  bool isAuthenticated = auth();
  if (!isAuthenticated) {
    // TODO Implement buzzer and stuff
    buzz();
    sendMessage("Not Authenticated!");
    userCart = "";
    return;
  }

  int cost = getProductCost(userCart);
  if (cost > userBalance) {
    buzz();
    sendMessage("Not Balanced!!");
    userCart = "";  
    return;
  }

  userBalance -= cost;
  myservo.write(90);
  digitalWrite(LEDPin, HIGH);
  for (int pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);                   
      delay(5);
  }
  digitalWrite(LEDPin, LOW);
  
}

bool auth() {
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println("Tag " + tag + " entered.");
    bool wasTagValid = isTagValid(tag);
    
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return wasTagValid;
  }
}

bool isTagValid(String tag) {
  if (tag == "130610027") {
    Serial.println("Validdd");
    return true;
  }
  else if (tag == "1631422827"){
    Serial.println("Not Validdd");
    return false;
  }
  Serial.println("nabudddd");
  return false;
}

void buzz() {
  // Change duration if necessary
  tone(buzzPin, 1000, 2000); delay(150);
}

int getProductCost(String product) {
  if (product == "milk") {
    return 7000;
  }
  else if (product == "soda") {
    return 5000;
  }
  else if (product == "peanut_pack") {
    return 4000;
  }
  else if (product == "chips") {
    return 6000;
  }
  return 0;
}

void sendMessage(String message) {
  if (globalClient == NULL) {
    Serial.println("ClIENT=NULL: Could not send message: " + message);
    return;
  }
  if (globalClient->status() != WS_CONNECTED) {
    Serial.println("ClIENT=DISCONNECTED: Could not send message: " + message);
    return;
  }
  
  globalClient->text(message);
}
