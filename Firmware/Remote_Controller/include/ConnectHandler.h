#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "Package.h"
#include "lvgl.h"
#include "gui.h"
#include "global.h"

#define SERVER_PORT                 20001
#define MATCH_PORT                  20002

#define MATCH_TIMEOUT_MS            5000
#define TIMEOUT_MS                  3000

#define BUF_SIZE(x)                 (x[2])

using namespace std;

class ConnectHandler {
public:
    void begin(void);
    void process(void);
    static void task(void* args);

private:
    Package _package;
    bool _connected;
    bool _matched;
    
    bool _match(void);
    bool _transmit(void);
    void _construct_transaction_data(msg_request_t& msg);
    static void _wifi_event_cb(arduino_event_id_t event, arduino_event_info_t info);
};

extern ConnectHandler connHandler;
