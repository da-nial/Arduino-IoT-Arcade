#include <ESP8266WiFi.h>

String newHostname = "NewESP8266Node";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);

  WiFi.begin("Danial", "ssssssssss");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(WiFi.status());
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());

  WiFi.hostname(newHostname.c_str());

  Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:

}
