#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <cstring>

#include "Package.h"
#include "message_handler.h"

#define WIFI_SSID                   "TP-LINK_B596"
#define WIFI_PWR                    "chen1370292520"

#define SERVER_PORT                 20001
#define MATCH_PORT                  20002

#define MATCH_MAX_TIME              5000

#define BUF_SIZE(x)                 (x[2])

using namespace std;

class ConnectHandler {
public:
    ConnectHandler();
    void begin(void);
    void process(void);

private:
    WiFiClient _client;
    Package _package;
    bool _connected;
    IPAddress _stripIP;

    void _handleRequest(void);
    void _match(void);
};

extern ConnectHandler connHandler;
