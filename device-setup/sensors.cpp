#include "sensors.h"

#define DHTTYPE DHT22
#define DHTPIN 13

// LED pins
const int redTempLedPin = 2;
const int greenTempLedPin = 3;
const int yellowHumidLedPin = 6;
const int blueHumidLedPin = 7;

DHT dht(DHTPIN, DHTTYPE);

static unsigned long lastRead = 0;

sensors::sensors() {
  _temperature = 10;
  _humidity = 10;
}

void sensors::begin() {
  initializeDHT22();
}

void sensors::initializeDHT22() {
  dht.begin();

  pinMode(redTempLedPin, OUTPUT);
  pinMode(greenTempLedPin, OUTPUT);
  pinMode(yellowHumidLedPin, OUTPUT);
  pinMode(blueHumidLedPin, OUTPUT);

  _temperature = NAN;
  _humidity = NAN;
}

void sensors::update() {
  if (millis() - lastRead < 2000) return;
  lastRead = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 read failed");
    return;
  }

  _temperature = temperature;
  _humidity = humidity;

  // Temperatur LED
  digitalWrite(redTempLedPin, _temperature < 20);
  digitalWrite(greenTempLedPin, _temperature >= 20);

  // Fugt LED
  digitalWrite(blueHumidLedPin, _humidity >= 40);
  digitalWrite(yellowHumidLedPin, _humidity < 40);
}

float sensors::getTemperature() {
  return _temperature;
}

float sensors::getHumidity() {
  return _humidity;
}
