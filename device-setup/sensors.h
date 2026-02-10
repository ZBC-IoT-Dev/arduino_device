#pragma once
#include <Arduino.h>
#include <DHT.h>

class sensors {
public:
  sensors();

  void begin();
  void update();

  float getTemperature();
  float getHumidity();

private:
  void initializeDHT22();

  float _temperature;
  float _humidity;
};