#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#define LED_NUM                 30
#define LED_PIN                 18

#define QUEUE_TIMEOUT_MS        (20)
#define DEBUG_LOG               0

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

#define SSID_MAX_LEN            32
#define PSW_MAX_LEN             32

typedef struct {
    char ssid[SSID_MAX_LEN];
    char password[PSW_MAX_LEN];
} connectivity_t;

extern NeoPixelBusType strip;
