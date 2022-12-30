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

#define MATCH_TIMEOUT_MS            5000
#define TIMEOUT_MS                  3000

#define BUF_SIZE(x)                 (x[2])

using namespace std;

class ConnectHandler {
public:
    ConnectHandler();
    void begin(void);
    void process(void);
    static led_mode_t packMode(uint8_t mode);
    static ease_t packEase(uint8_t ease);
    static faded_end_t packFadedEnd(uint8_t faded_end);
    static dir_t packDirection(uint8_t dir);

private:
    Package _package;
    bool _connected;
    IPAddress _hostIp;

    void _handle(void);
    bool _match(void);
    bool _transmit(void);
    void _pack(msg_struct_t& msg);
    void _parse(msg_reply_t& reply);
};

extern ConnectHandler connHandler;
