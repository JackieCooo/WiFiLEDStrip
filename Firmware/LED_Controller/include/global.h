#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "freertos/queue.h"
#include "lwip/ip4_addr.h"

#define LED_NUM             30
#define LED_PIN             18

#define QUEUE_TIMEOUT_MS    (20)

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

// typedef struct {
//     bool power;
//     led_mode_t mode;
//     struct {
//         struct {
//             RgbColor color;
//         } normal;
//         struct {
//             RgbColor color;
//             uint16_t duration;
//             uint16_t interval;
//             ease_t ease;
//         } breathing;
//         struct {
//             RgbColor color;
//             uint8_t len;
//             uint8_t gap;
//             faded_end_t faded_end;
//             uint8_t head_len;
//             uint8_t tail_len;
//             dir_t dir;
//             uint16_t speed;
//         } lightbeam;
//         struct {
//             uint16_t speed;
//         } rainbow;
//     } setting;
// } configuration_t;

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
