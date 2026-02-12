#include <Arduino.h>
#include "sensors.h"
#include "network.h"

sensors sensors;
network network;

//¤=========================================================================================¤

void setup() {
  Serial.begin(9600);
  sensors.begin();
  network.begin();
}

//¤=========================================================================================¤

void loop() {
  if (!network.client.connected()) {
   network.connectMQTT();
  }
  network.client.loop();

  //--CLIMATESENSOR START--
  sensors.updateDHT();

  float temperature = sensors.getTemperature();
  float humidity    = sensors.getHumidity();

  // (valgfrit) brug værdierne – fx MQTT
  if (!isnan(temperature) && !isnan(humidity)) {
    String climateJson =
      "{\"id\":\"arduino_stue_1\",\"type\":\"Climate\","
      "\"temperature\":" + String(temperature) +
      ",\"humidity\":" + String(humidity) + "}";

    network.client.publish("sensors/climate", climateJson.c_str());
  }

  Serial.println("temp: " + String(sensors.getTemperature()) + " | humid: " + String(sensors.getHumidity()));
  //--CLIMATESENSOR END--

  sensors.updatePIR();

  // Create the Discovery JSON
  // id: unique hardware ID, type: sensor category
  String jsonPayload = "{\"id\": \"arduino_stue_1\", \"type\": \"MotionSensor\", \"detectionStatus\":" + String(sensors.getDetectionStatus()) + "}";
  //String jsonPayload = "{\"id\": \"arduino_stue_1\", \"type\": \"MotionSensor\", \"isDetected\":" + String(isDetected ? "true" : "false") + "}";

  Serial.println("Sending discovery pulse...");
  Serial.println("discovery/announce" + String(jsonPayload));
  network.client.publish("discovery/announce", jsonPayload.c_str());

  // Wait 0,5 seconds before next shout
  delay(500);
}

//¤=========================================================================================¤
