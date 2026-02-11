#ifndef NETWORK_H
#define NETWORK_H
#include <WiFiS3.h>
#include <PubSubClient.h>


class network {
public:
  network();
  void begin();
  void networkConnect();

private:
  char* _ssid;
  char* _password;
};
#endif