#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "freertos/queue.h"

#define REFRESH_GUI                     1
#define RES_MATCH                       2

#define DEFAULT_COLOR                   lv_palette_main(LV_PALETTE_RED)
#define QUEUE_TIMEOUT_MS                (20)

typedef struct {
    uint16_t color;
} normal_data_t;

typedef struct {
    uint16_t color;
    uint16_t duration;
    uint16_t interval;
    uint8_t ease;
} breathing_data_t;

typedef struct {
    uint16_t color;
    uint16_t len;
    uint16_t gap;
    uint16_t head_len;
    uint16_t tail_len;
    uint16_t speed;
    uint8_t faded_end;
    uint8_t dir;
} lightbeam_data_t;

typedef struct {
    uint16_t speed;
} rainbow_data_t;

typedef union {
    normal_data_t normal;
    breathing_data_t breathing;
    lightbeam_data_t lightbeam;
    rainbow_data_t rainbow;
} setting_data_t;

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW
} led_mode_t;

typedef enum {
    EASE_LINEAR,
    EASE_QUADRATIC_IN,
    EASE_QUADRATIC_OUT,
    EASE_QUADRATIC_IN_OUT,
    EASE_QUADRATIC_CENTER,
    EASE_CUBIC_IN,
    EASE_CUBIC_OUT,
    EASE_CUBIC_IN_OUT,
    EASE_CUBIC_CENTER,
    EASE_QUARTIC_IN,
    EASE_QUARTIC_OUT,
    EASE_QUARTIC_IN_OUT,
    EASE_QUARTIC_CENTER,
    EASE_QUINTIC_IN,
    EASE_QUINTIC_OUT,
    EASE_QUINTIC_IN_OUT,
    EASE_QUINTIC_CENTER,
    EASE_SINUSOIDAL_IN,
    EASE_SINUSOIDAL_OUT,
    EASE_SINUSOIDAL_IN_OUT,
    EASE_SINUSOIDAL_CENTER,
    EASE_EXPONENTIAL_IN,
    EASE_EXPONENTIAL_OUT,
    EASE_EXPONENTIAL_IN_OUT,
    EASE_EXPONENTIAL_CENTER,
    EASE_CIRCULAR_IN,
    EASE_CIRCULAR_OUT,
    EASE_CIRCULAR_IN_OUT,
    EASE_CIRCULAR_CENTER,
    EASE_GAMMA
} ease_t;

typedef struct {
    uint8_t FADED_HEAD : 1;
    uint8_t FADED_TAIL : 1;
} faded_end_t;

typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT
} dir_t;

typedef struct {
    bool power;
    led_mode_t mode;
    struct {
        struct {
            lv_color_t color;
        } normal;
        struct {
            lv_color_t color;
            uint16_t duration;
            uint16_t interval;
            ease_t ease;
        } breathing;
        struct {
            lv_color_t color;
            uint16_t len;
            uint16_t gap;
            uint16_t head_len;
            uint16_t tail_len;
            uint16_t speed;
            faded_end_t faded_end;
            dir_t dir;
        } lightbeam;
        struct {
            uint16_t speed;
        } rainbow;
    } setting;
} configuration_t;

typedef enum {
    MSG_READ_CONFIG,
    MSG_WRITE_CONFIG,
    MSG_MATCH,
    MSG_CONNECT,
} msg_t;

typedef struct {
    msg_t msg;
    void* user_data;
} msg_request_t;

typedef struct {
    msg_t msg;
    void* user_data;
    bool resp;
} msg_reply_t;

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
} ip_addr_t;

typedef struct {
    bool connected;
    bool matched;
    ip_addr_t host_ip;
} connectivity_t;

extern configuration_t configuration;
extern xQueueHandle messageHandler;
extern xQueueHandle saveConfigMessage;
extern connectivity_t connectivity;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
