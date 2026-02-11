#include "network.h"


const char* _ssid     = "DEV";
const char* _password = "Kaffe10ko";

WiFiClient wificlient;
PubSubClient networkClient(wificlient);

network::network(){

}

void network::begin(){
  networkConnect();
}

void network::networkConnect(){
  Serial.print("Connecting to WiFi...");
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected!");
}