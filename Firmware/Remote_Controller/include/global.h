#ifndef _GLOBAL_H
#define _GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "lvgl.h"

#define RES_CONFIG_UPDATED              1
#define RES_MATCH                       2

#define DEFAULT_POWER                   false
#define DEFAULT_MODE                    MODE_NORMAL
#define DEFAULT_COLOR                   lv_color_hex(0x001f)
#define DEFAULT_DURATION                2000
#define DEFAULT_INTERVAL                1000
#define DEFAULT_EASE                    EASE_LINEAR
#define DEFAULT_LEN                     3
#define DEFAULT_SPEED                   8
#define DEFAULT_FADED_END               FADED_DISABLE
#define DEFAULT_HEAD_LEN                4
#define DEFAULT_TAIL_LEN                4
#define DEFAULT_DIRECTION               MOVE_RIGHT
#define DEFAULT_GAP                     2

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

typedef uint8_t faded_end_t;

#define FADED_DISABLE               (0)
#define FADED_HEAD                  (1 << 0)
#define FADED_TAIL                  (1 << 1)

typedef enum {
    MOVE_RIGHT,
    MOVE_LEFT
} dir_t;

typedef struct {
    bool power;
    led_mode_t mode;
    union {
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

extern configuration_t configuration;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
