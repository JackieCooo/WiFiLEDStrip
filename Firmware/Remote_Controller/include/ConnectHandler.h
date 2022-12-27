#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <cstring>
#include <queue>

#include "Package.h"

#define WIFI_SSID                   "TP-LINK_B596"
#define WIFI_PWR                    "chen1370292520"

#define SERVER_PORT                 20001
#define MATCH_PORT                  20002

#define MATCH_MAX_TIME              5000

typedef enum {
    MSG_MODE_CHANGED,
    MSG_COLOR_CHANGED,
    MSG_DURATION_CHANGED,
    MSG_INTERVAL_CHANGED,
    MSG_SPEED_CHANGED,
    MSG_LEN_CHANGED,
    MSG_FADED_END_CHANGED,
    MSG_TAIL_LEN_CHANGED,
    MSG_HEAD_LEN_CHANGED,
    MSG_DIRECTION_CHANGED,
    MSG_EASE_CHANGED
} msg_t;

typedef struct {
    msg_t msg;
    uint8_t mode;
    union {
        struct {
            uint16_t color;
        } normal;
        struct {
            uint16_t color;
            uint16_t duration;
            uint16_t interval;
            uint8_t ease;
        } breathing;
        struct {
            uint16_t color;
            uint16_t len;
            uint16_t interval;
            uint16_t head_len;
            uint16_t tail_len;
            uint16_t speed;
            uint8_t faded_end;
            uint8_t dir;
        } lightbeam;
        struct {
            uint16_t speed;
        } rainbow;
    } setting;
} msg_struct_t;

using namespace std;

class ConnectHandler {
public:
    ConnectHandler();
    void begin(void);
    void process(void);
    void pushMsg(msg_struct_t msg);

private:
    WiFiClient _client;
    Package _package;
    bool _connected;
    IPAddress _stripIP;
    queue<msg_struct_t> _msgQueue;

    void _handleRequest(void);
    void _match(void);
};

extern ConnectHandler connHandler;
