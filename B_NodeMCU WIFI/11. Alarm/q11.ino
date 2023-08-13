#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"
#include <TimeLib.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Danial";
const char *password = "ssssssssss";

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP);

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);

uint32_t timeToAlarm;
int alarmFreq;

long snoozeDuration = 5;

const int buzzPin = D0;

void setup() {
  Serial.begin(115200);
  delay(3000);
  setupWIFI();
  setupNTP();
  setupBuzzer();
  setupAccessPoint();
  setupWebServer();

  timeToAlarm = -1;
}

void loop() {
  timeClient.update();
  alarm();
}

String processor(const String& var) {
  return String();
}

void setupWIFI() {
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
}

void setupAccessPoint(){
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void setupBuzzer() {
  pinMode(buzzPin, OUTPUT);
}

void setupNTP() {
  timeClient.begin();
  timeClient.setTimeOffset(16200);
  timeClient.update();
}

void setupWebServer() {
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", webpage, processor);
  });

  server.on("/set-alarm", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // Extract time to alarm value from request body
    if (request->hasParam("time_input") && request->hasParam("freq")) {
        String formattedAlarmTime = request->getParam("time_input")->value();
        int freq = request->getParam("freq")->value().toInt();
        setAlarm(formattedAlarmTime, freq);
    }
    request->send_P(200, "text/html", webpage, processor);
  });

  server.on("/stop", HTTP_GET, [] (AsyncWebServerRequest *request) {
    stopAlarm();
    request->send_P(200, "text/html", webpage, processor);
  });
  

  server.on("/snooze", HTTP_GET, [] (AsyncWebServerRequest *request) {
    snoozeAlarm();
    request->send_P(200, "text/html", webpage, processor);
  });

  server.begin();
}

void alarm() {
  if (timeToAlarm == -1) {
    return;
  }
  
  if (timeClient.getEpochTime() > timeToAlarm) {
    buzz();
  }
}

void buzz() {
  // Change duration if necessary
  tone(buzzPin, alarmFreq, 1000); delay(150);
}

void setAlarm(String formattedAlarmTime, int freq) {
  timeToAlarm = parseFormattedTime(formattedAlarmTime);
  alarmFreq = freq;

  Serial.print("Alarm Set for ");
  Serial.print(formattedAlarmTime);
  Serial.print(" with frequency ");
  Serial.println(freq);
}

void stopAlarm() {
  timeToAlarm = -1;
  Serial.println("Alarm Stopped!");
}

void snoozeAlarm() {
  if (timeToAlarm == -1) {
    return;
  }

  timeToAlarm = timeClient.getEpochTime() + snoozeDuration;
  Serial.print("Alarm snoozed for ");
  Serial.println(timeToAlarm);
}

time_t parseFormattedTime(String formattedTime) {
  TimeElements timeElements;
  int h, m, s;
  sscanf(formattedTime.c_str(), "%2d:%2d:%2d", &h, &m, &s);
  
  //Get a time structure
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int currentDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900-1970;

  timeElements.Year = currentYear;
  timeElements.Month = currentMonth;
  timeElements.Day = currentDay;
  timeElements.Hour = h;
  timeElements.Minute = m;
  timeElements.Second = s;
  time_t t = makeTime(timeElements);

  return t;
}
