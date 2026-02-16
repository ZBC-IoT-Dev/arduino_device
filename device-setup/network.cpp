#include "network.h"

const char* _wifi_ssid = "DEV";
const char* _wifi_pass = "Kaffe10ko";
const char* _mqtt_server = "10.106.189.237"; // Example: 192.168.1.50

WiFiClient wificlient;
PubSubClient client(wificlient);

//¤=========================================================================================¤

network::network(FancyLog& fancyLog) : fancyLog(fancyLog) {}


void network::begin() {
  connectWiFi();
  client.setServer(_mqtt_server, 1883); // Setup MQTT
  fancyLog.toSerial("Network initialized");
}


void network::connectWiFi() {
  Serial.print("| Connecting to WiFi...");

  WiFi.begin(_wifi_ssid, _wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n| WiFi connected!");
}


void network::connectMQTT() {
  while (!client.connected()) {
    Serial.print("| Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ArduinoClient-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" - trying again in 5 seconds");
      delay(5000);
    }
  }
}

//¤=========================================================================================¤
