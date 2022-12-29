#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <cstring>

#include "Package.h"
#include "message_handler.h"
#include "lvgl.h"
#include "global.h"

#define WIFI_SSID                   "TP-LINK_B596"
#define WIFI_PWR                    "chen1370292520"

#define SERVER_PORT                 20001
#define MATCH_PORT                  20002

#define MATCH_MAX_TIME              5000
#define TIMEOUT_MS                  2000

#define BUF_SIZE(x)                 (x[2])

using namespace std;

class ConnectHandler {
public:
    ConnectHandler();
    void begin(void);
    void process(void);

private:
    Package _package;
    bool _connected;
    IPAddress _hostIp;

    void _handle(void);
    void _match(void);
    bool _transmit(uint8_t* buf, uint8_t size);
};

extern ConnectHandler connHandler;
