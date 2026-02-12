#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>

class sensors {
  public:
    sensors();
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
};
#endif