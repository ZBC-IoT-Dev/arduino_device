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
//int state = LOW;
//int value = 0;
//bool isDetected = false;

DHT dht(DHTPIN, DHTTYPE);

static unsigned long lastRead = 0;

sensors::sensors() {
  _temperature = 10;
  _humidity = 10;
}

void sensors::begin() {
  initializeDHT22();

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  Serial.println("Sensors initialized");
}

//¤=========================================================================================¤

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

//¤=========================================================================================¤

/*void sensors::initializePIR(){
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
}*/
