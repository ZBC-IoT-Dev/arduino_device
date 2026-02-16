#include "sensors.h"

#define DHTTYPE DHT22
#define DHTPIN 13

// LED pins
const int redTempLedPin = 2;
const int greenTempLedPin = 3;
const int yellowHumidLedPin = 6;
const int blueHumidLedPin = 7;
const int ledPin = 9;

//PIR pins
const int pirPin = 5;
int state = LOW;
int value = 0;
bool _detectionStatus;

//DHT dht(DHTPIN, DHTTYPE);

static unsigned long lastRead = 0;

//¤=========================================================================================¤

sensors::sensors(FancyLog& fancyLog) : fancyLog(fancyLog), dht(DHTPIN, DHTTYPE) {
  _temperature = 10;
  _humidity = 10;
}


void sensors::begin() {
  initializeDHT();
  initializePIR();
}

//¤=========================================================================================¤

void sensors::initializeDHT() {
  dht.begin();

  pinMode(redTempLedPin, OUTPUT);
  pinMode(greenTempLedPin, OUTPUT);
  pinMode(yellowHumidLedPin, OUTPUT);
  pinMode(blueHumidLedPin, OUTPUT);

  _temperature = NAN;
  _humidity = NAN;

  fancyLog.toSerial("DHT sensor initialized");
}


void sensors::updateDHT() {
  if (millis() - lastRead < 2000) return;
  lastRead = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    fancyLog.toSerial("DHT22 read failed", ERROR);
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

//¤=========================================================================================¤

void sensors::initializePIR() {
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  fancyLog.toSerial("PIR sensor initialized");
}


void sensors::updatePIR() {
  value = digitalRead(pirPin);

  if (value == HIGH){
    digitalWrite(ledPin, HIGH);
    delay(500);

    if(state == LOW){
      fancyLog.toSerial("Motion detected", INFO);
      state = HIGH;
      _detectionStatus = value;
    }
  } else {
    digitalWrite(ledPin, LOW);
    delay(500);

    if(state == HIGH){
      fancyLog.toSerial("The motion has stopped", INFO);
      state = LOW;
      _detectionStatus = value;
    }
  }
}


bool sensors::getDetectionStatus() {
  return _detectionStatus;
}

//¤=========================================================================================¤
