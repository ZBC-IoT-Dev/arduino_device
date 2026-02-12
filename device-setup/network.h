#ifndef NETWORK_H
#define NETWORK_H

#include <WiFiS3.h>
#include <PubSubClient.h>

class network {
  public:
    network();
    void begin();
    void connectWiFi();
    void connectMQTT();
    PubSubClient client;

  private:
    char* _wifi_ssid;
    char* _wifi_pass;
    char* _mqtt_server;
    WiFiClient wificlient;
};
#endif