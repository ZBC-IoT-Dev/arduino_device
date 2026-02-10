#include <Arduino.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "sensors.h"

// --- CONFIGURATION ---
const char* ssid     = "DEV";
const char* password = "Kaffe10ko";
const char* mqtt_server = "10.106.189.237"; // Example: 192.168.1.50
const int ledPin = 9;
const int pirPin = 5;
int state = LOW;
int value = 0;
bool isDetected = false;

sensors climate;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  climate.begin();

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected!");

  // Setup MQTT
  client.setServer(mqtt_server, 1883);
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ArduinoClient-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" - trying again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  //if (!client.connected()) {
  // connectMQTT();
  //}
  //client.loop();

  //--CLIMATESENSOR START--
  climate.update();

  float temperature = climate.getTemperature();
  float humidity    = climate.getHumidity();

  // (valgfrit) brug værdierne – fx MQTT
  if (!isnan(temperature) && !isnan(humidity)) {
    String climateJson =
      "{\"id\":\"arduino_stue_1\",\"type\":\"Climate\","
      "\"temperature\":" + String(temperature) +
      ",\"humidity\":" + String(humidity) + "}";

    client.publish("sensors/climate", climateJson.c_str());
  }

  Serial.println("temp: " + String(climate.getTemperature()) + " | humid: " + String(climate.getHumidity()));

  //--CLIMATESENSOR END--

  value = digitalRead(pirPin);

  if (value == HIGH){
    digitalWrite(ledPin, HIGH);
    delay(500);

    if(state == LOW){
      Serial.println("Motion detected");
      state = HIGH;
      isDetected = value;
    }
  } else {
    digitalWrite(ledPin, LOW);
    delay(500);

    if(state == HIGH){
      Serial.println("The motion has stopped");
      state = LOW;
      isDetected = value;
    }
  }

  // Create the Discovery JSON
  // id: unique hardware ID, type: sensor category
  String jsonPayload = "{\"id\": \"arduino_stue_1\", \"type\": \"MotionSensor\", \"isDetected\":" + String(isDetected ? "true" : "false") + "}";

  Serial.println("Sending discovery pulse...");
  Serial.println("discovery/announce" + String(jsonPayload));
  client.publish("discovery/announce", jsonPayload.c_str());

  // Wait 0,5 seconds before next shout
  delay(500);
}