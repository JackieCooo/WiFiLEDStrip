#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <string.h>

#include "Package.h"
#include "StripHandler.h"
#include "global.h"

#define WIFI_SSID               "TP-LINK_B596"
#define WIFI_PWR                "chen1370292520"
#define SERVER_PORT             20001
#define MATCH_PORT              20002
#define TIMEOUT_MS              5000

#define BUF_SIZE(x)             (x[2])

class ConnectHandler {
public:
    void begin(void);
    void process(void);
    static void task(void* args);
private:
    WiFiServer _server;
    Package _package;

    void _handle(WiFiClient& client);
};

extern ConnectHandler connHandler;
