#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "global.h"

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
} fade_t;

typedef enum {
    DIR_BACKWARD,
    DIR_FORWARD,
} dir_t;

typedef struct {
    lv_color_t color;
} normal_data_t;

typedef struct {
    lv_color_t color;
    uint16_t duration;
    uint16_t interval;
    ease_t ease;
} breathing_data_t;

typedef struct {
    lv_color_t color;
    uint8_t len;
    uint8_t gap;
    fade_t fade;
    uint8_t head;
    uint8_t tail;
    dir_t dir;
    uint16_t speed;
} lightbeam_data_t;

typedef struct {
    uint16_t speed;
} rainbow_data_t;

typedef struct {
    bool power;
    led_mode_t mode;
    normal_data_t normal;
    breathing_data_t breathing;
    lightbeam_data_t lightbeam;
    rainbow_data_t rainbow;
} config_t;

void config_set_power(bool pwr);
bool config_get_power(void);
void config_set_mode(led_mode_t mode);
led_mode_t config_get_mode(void);
void config_set_normal_data(const normal_data_t* data);
normal_data_t config_get_normal_data(void);
void config_set_breathing_data(const breathing_data_t* data);
breathing_data_t config_get_breathing_data(void);
void config_set_lightbeam_data(const lightbeam_data_t* data);
lightbeam_data_t config_get_lightbeam_data(void);
void config_set_rainbow_data(const rainbow_data_t* data);
rainbow_data_t config_get_rainbow_data(void);

led_mode_t translate_mode(uint8_t val);
ease_t translate_ease(uint8_t val);
fade_t val_to_fade(uint8_t val);
uint8_t fade_to_val(fade_t fade);
dir_t translate_dir(uint8_t val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
