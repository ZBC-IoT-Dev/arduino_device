// #include <Arduino.h>
// // #include <WiFiS3.h>
// //#include <PubSubClient.h>
// #include "sensors.h"
// #include "network.h"

// // --- CONFIGURATION ---
// // const char* ssid     = "DEV";
// // const char* password = "Kaffe10ko";
// const char* mqtt_server = "10.106.189.237"; // Example: 192.168.1.50

// // DELETE LATER
// const int pirPin = 5;
// int state = LOW;
// int value = 0;
// bool isDetected = false;
// const int ledPin = 9;
// // ### ### ###

// sensors climate;
// network network;

// //WiFiClient espClient;
// //PubSubClient client(espClient);

// void setup() {
//   Serial.begin(9600);
//   climate.begin();
//   network.begin();
//   // Connect to WiFi
//   // Serial.print("Connecting to WiFi...");
//   // WiFi.begin(ssid, password);
//   // while (WiFi.status() != WL_CONNECTED) {
//   //   delay(500);
//   //   Serial.print(".");
//   // }
//   // Serial.println("\n WiFi Connected!");

//   // Setup MQTT
//   client.setServer(mqtt_server, 1883);
// }

// void connectMQTT() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Create a random client ID
//     String clientId = "ArduinoClient-" + String(random(0xffff), HEX);
    
//     if (client.connect(clientId.c_str())) {
//       Serial.println("connected!");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" - trying again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

// void loop() {
//   if (!client.connected()) {
//    connectMQTT();
//   }
//   client.loop();

//   //--CLIMATESENSOR START--
//   climate.update();

//   float temperature = climate.getTemperature();
//   float humidity    = climate.getHumidity();

//   // (valgfrit) brug værdierne – fx MQTT
//   if (!isnan(temperature) && !isnan(humidity)) {
//     String climateJson =
//       "{\"id\":\"arduino_stue_1\",\"type\":\"Climate\","
//       "\"temperature\":" + String(temperature) +
//       ",\"humidity\":" + String(humidity) + "}";

//     client.publish("sensors/climate", climateJson.c_str());
//   }

//   Serial.println("temp: " + String(climate.getTemperature()) + " | humid: " + String(climate.getHumidity()));

//   //--CLIMATESENSOR END--

//   value = digitalRead(pirPin);

//   if (value == HIGH){
//     digitalWrite(ledPin, HIGH);
//     delay(500);

//     if(state == LOW){
//       Serial.println("Motion detected");
//       state = HIGH;
//       isDetected = value;
//     }
//   } else {
//     digitalWrite(ledPin, LOW);
//     delay(500);

//     if(state == HIGH){
//       Serial.println("The motion has stopped");
//       state = LOW;
//       isDetected = value;
//     }
//   }

//   // Create the Discovery JSON
//   // id: unique hardware ID, type: sensor category
//   String jsonPayload = "{\"id\": \"arduino_stue_1\", \"type\": \"MotionSensor\", \"isDetected\":" + String(isDetected ? "true" : "false") + "}";

//   Serial.println("Sending discovery pulse...");
//   Serial.println("discovery/announce" + String(jsonPayload));
//   client.publish("discovery/announce", jsonPayload.c_str());

//   // Wait 0,5 seconds before next shout
//   delay(500);
// }

#include <Arduino.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <WDT.h>
#include "sensors.h"
#include "network.h"

sensors climate;
network net;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const char* mqtt_server = "10.106.189.237";

const int pirPin = 5;
const int ledPin = 9;

bool lastPirState = LOW;

// MQTT reconnect timing
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000;

void handleMQTT();
void handlePIR();

void setup() {
  Serial.begin(9600);

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  climate.begin();
  net.begin();

  client.setServer(mqtt_server, 1883);

  // Watchdog 8 sek
  WDT.begin(8000);
}

void loop() {

  // Feed watchdog
  WDT.refresh();

  net.handleWiFi();
  handleMQTT();
  client.loop();

  climate.update();
  handlePIR();
}

/* ================= MQTT (non-blocking) ================= */

void handleMQTT() {

  if (client.connected()) return;

  if (millis() - lastReconnectAttempt > reconnectInterval) {
    lastReconnectAttempt = millis();

    Serial.println("Attempting MQTT connection...");

    if (client.connect("arduino_stue_1")) {
      Serial.println("MQTT connected");
    } else {
      Serial.println("MQTT failed");
    }
  }
}

/* ================= PIR uden delay ================= */

void handlePIR() {

  bool currentState = digitalRead(pirPin);

  if (currentState != lastPirState) {

    lastPirState = currentState;

    digitalWrite(ledPin, currentState);

    if (currentState == HIGH) {
      Serial.println("Motion detected");
    } else {
      Serial.println("Motion stopped");
    }
  }
}
