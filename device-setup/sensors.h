#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "FancyLog.h"

class sensors {
  public:
    sensors(FancyLog& fancyLog);
    void begin();
    void updateDHT();
    void updatePIR();
    float getTemperature();
    float getHumidity();
    bool getDetectionStatus();

  private:
    void initializeDHT();
    void initializePIR();
    float _temperature;
    float _humidity;
    bool _detectionStatus;
    DHT dht;
    FancyLog& fancyLog;
};

#endif