#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();

  scanLocalNetworks();
  connectToNetwork();
}

void loop() {
}

void scanLocalNetworks(){
    // scan for nearby networks:
  Serial.println("** Scannig Networks **");
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numNetworks);

  // print the network info for each network:
  for (int net = 0; net < numNetworks; net++) {
    Serial.print(net);
    Serial.print(": ");
    Serial.print(WiFi.SSID(net));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(net));
    Serial.println(" dBm");
    Serial.println("----------------------------");
  }
  Serial.println("****************************");
}

void connectToNetwork(){
  WiFi.mode(WIFI_STA);

  WiFi.begin("Danial", "ssssssssss");

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(WiFi.status());
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
