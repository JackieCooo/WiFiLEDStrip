#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Package.h>
#include <StripHandler.h>
#include <string.h>

#include "global_conf.h"

#define WIFI_SSID "TP-LINK_B596"
#define WIFI_PWR "chen1370292520"
#define SERVER_PORT 20001

class ConnectHandler {
public:
    void begin(void);
    void process(void);

private:
    WiFiServer _server;
    Package _package;

    void _handleRequest(void);
};

extern ConnectHandler connHandler;
