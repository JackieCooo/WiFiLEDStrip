#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <string.h>

#include "Package.h"
#include "StripHandler.h"
#include "global.h"

#define WIFI_SSID               "HUAWEI-10GJ0E"
#define WIFI_PWR                "abc12345678"
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
    WiFiUDP _matcher;

    void _handle(WiFiClient& client, package_t& pack);
};

extern ConnectHandler connHandler;
