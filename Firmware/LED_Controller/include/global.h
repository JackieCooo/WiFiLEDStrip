#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "freertos/queue.h"

#define LED_NUM             30
#define LED_PIN             18

#define QUEUE_TIMEOUT_MS    (20)

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

typedef enum {
    MSG_READ_CONFIG,
    MSG_WRITE_CONFIG,
    MSG_MATCH,
} msg_t;

typedef struct {
    msg_t msg;
    void* user_data;
} msg_request_t;

typedef enum {
    FILE_CONFIG,
    FILE_CONNECT,
} local_file_t;

#define SSID_MAX_LEN            32
#define PSW_MAX_LEN             32

typedef struct {
    char ssid[SSID_MAX_LEN];
    char password[PSW_MAX_LEN];
} connectivity_t;

extern connectivity_t connectivity;
extern xQueueHandle saveFileMessage;
extern NeoPixelBusType strip;
