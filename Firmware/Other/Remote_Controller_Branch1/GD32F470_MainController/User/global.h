#ifndef _GLOBAL_H
#define _GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "lvgl.h"

#define DEFAULT_MODE				MODE_NORMAL
#define DEFAULT_POWER				false
#define DEFAULT_COLOR				lv_palette_main(LV_PALETTE_BLUE)
#define DEFAULT_DURATION			2000
#define DEFAULT_INTERVAL			1000
#define DEFAULT_EASE				EASE_LINEAR
#define DEFAULT_LEN					2
#define DEFAULT_GAP					2
#define DEFAULT_HEAD_LEN			4
#define DEFAULT_TAIL_LEN			4
#define DEFAULT_SPEED				8
#define DEFAULT_FADED_END			3
#define DEFAULT_DIR					MOVE_LEFT

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
            uint8_t len;
            uint8_t gap;
            faded_end_t faded_end;
            uint8_t head_len;
            uint8_t tail_len;
            dir_t dir;
            uint16_t speed;
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
