#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#define LED_NUM 30
#define LED_PIN 18

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

typedef struct {
    uint8_t power;
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
            uint16_t gap;
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
} configuration_t;

extern configuration_t configuration;
extern NeoPixelBusType strip;
