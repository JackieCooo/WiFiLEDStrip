#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Package.h>
#include <StripHelper.h>
#include <string.h>

#include "global_conf.h"

#define WIFI_SSID "TP-LINK_B596"
#define WIFI_PWR "chen1370292520"
#define SERVER_PORT 20001

#define RGB565_R(x) ((uint8_t)((x & 0xF800) >> 11))
#define RGB565_G(x) ((uint8_t)((x & 0x07E0) >> 5))
#define RGB565_B(x) ((uint8_t)((x & 0x001F)))

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
