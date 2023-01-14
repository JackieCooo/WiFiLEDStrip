#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#define LED_NUM 30
#define LED_PIN 18

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW,
    MODE_NUM
} strip_mode_t;

typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT
} dir_t;

typedef struct {
    uint8_t FADED_HEAD : 1;
    uint8_t FADED_TAIL : 1;
} faded_end_t;

typedef struct {
    RgbColor color;
} normal_data_t;

typedef struct {
    RgbColor color;
    uint16_t duration;
    uint16_t interval;
    AnimEaseFunction ease;
} breathing_data_t;

typedef struct {
    RgbColor color;
    uint16_t len;
    uint16_t gap;
    uint16_t head_len;
    uint16_t tail_len;
    faded_end_t faded_end;
    dir_t dir;
    uint16_t speed;
} lightbeam_data_t;

typedef struct {
    uint16_t speed;
} rainbow_data_t;

typedef struct {
    bool power;
    strip_mode_t mode;
    union {
        normal_data_t normal;
        breathing_data_t breathing;
        lightbeam_data_t lightbeam;
        rainbow_data_t rainbow;
    } setting;
} configuration_t;

extern configuration_t configuration;
extern NeoPixelBusType strip;
