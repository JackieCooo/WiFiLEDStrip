#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "Package.h"
#include "lvgl.h"
#include "gui.h"
#include "global.h"
#include "message_handler.h"
#include "ConfigHandler.h"

#define SERVER_PORT                 20001
#define MATCH_PORT                  20002

#define MATCH_TIMEOUT_MS            5000
#define TIMEOUT_MS                  3000

#define BUF_SIZE(x)                 (x[2] + 4)

using namespace std;

class ConnectHandler {
public:
    void begin(void);
    void process(void);
    bool get_connect_status(void);
    bool get_match_status(void);
    void set_match_status(bool status);

    static void task(void* args);

private:
    Package _package;
    bool _matched;
    
    bool _match(void);
    bool _transmit(uint8_t cmd);
    
    static void _wifi_event_cb(arduino_event_id_t event, arduino_event_info_t info);
};

extern ConnectHandler connHandler;
