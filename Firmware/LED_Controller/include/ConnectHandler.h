#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiServer.h>

#define WIFI_SSID "TP-LINK_B596"
#define WIFI_PWR "chen1370292520"
#define SERVER_PORT 20001

class ConnectHandler {
public:
    void begin(void);
    void process(void);

private:
    WiFiServer _server;
};
