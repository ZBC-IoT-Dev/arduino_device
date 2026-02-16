// #include "network.h"


// const char* _ssid     = "DEV";
// const char* _password = "Kaffe10ko";

// WiFiClient wificlient;
// PubSubClient networkClient(wificlient);

// network::network(){

// }

// void network::begin(){
//   networkConnect();
// }

// void network::networkConnect(){
//   Serial.print("Connecting to WiFi...");
//   WiFi.begin(_ssid, _password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\n WiFi Connected!");
// }

#include "network.h"

const char* ssid     = "DEV";
const char* password = "Kaffe10ko";

unsigned long lastWiFiAttempt = 0;
const unsigned long wifiInterval = 10000;

network::network(){}

void network::begin(){
  WiFi.begin(ssid, password);
}

void network::handleWiFi(){

  if (WiFi.status() == WL_CONNECTED) return;

  if (millis() - lastWiFiAttempt > wifiInterval) {

    lastWiFiAttempt = millis();

    Serial.println("Reconnecting WiFi...");
    WiFi.begin(ssid, password);
  }
}
