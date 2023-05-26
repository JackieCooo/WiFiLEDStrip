#include "config.h"

static config_t config;

void config_set_power(bool pwr) {
    config.power = pwr;
}

bool config_get_power(void) {
    return config.power;
}

void config_set_mode(led_mode_t mode) {
    config.mode = mode;
}

led_mode_t config_get_mode(void) {
    return config.mode;
}

void config_set_normal_data(const normal_data_t* data) {
    memcpy(config.normal, data, sizeof(normal_data_t));
}

normal_data_t config_get_normal_data(void) {
    return config.normal;
}

void config_set_breathing_data(const breathing_data_t* data) {
    memcpy(config.breathing, data, sizeof(breathing_data_t));
}

breathing_data_t config_get_breathing_data(void) {
    return config.breathing;
}

void config_set_lightbeam_data(const lightbeam_data_t* data) {
    memcpy(config.lightbeam, data, sizeof(lightbeam_data_t));
}

lightbeam_data_t config_get_lightbeam_data(void) {
    return config.lightbeam;
}

void config_set_rainbow_data(const rainbow_data_t* data) {
    memcpy(config.rainbow, data, sizeof(rainbow_data_t));
}

rainbow_data_t config_get_rainbow_data(void) {
    return config.rainbow;
}

led_mode_t translate_mode(uint8_t val) {
    switch (val)
    {
        case 0:
            return MODE_NORMAL;
            break;
        case 1:
            return MODE_BREATHING;
            break;
        case 2:
            return MODE_LIGHTBEAM;
            break;
        case 3:
            return MODE_RAINBOW;
            break;
        default:
            break;
    }

    return MODE_NORMAL;
}

ease_t translate_ease(uint8_t val) {
    switch (val)
    {
        case 0:
            return EASE_LINEAR;
            break;
        case 1:
            return EASE_QUADRATIC_IN;
            break;
        case 2:
            return EASE_QUADRATIC_OUT;
            break;
        case 3:
            return EASE_QUADRATIC_IN_OUT;
            break;
        case 4:
            return EASE_QUADRATIC_CENTER;
            break;
        case 5:
            return EASE_CUBIC_IN;
            break;
        case 6:
            return EASE_CUBIC_OUT;
            break;
        case 7:
            return EASE_CUBIC_IN_OUT;
            break;
        case 8:
            return EASE_CUBIC_CENTER;
            break;
        case 9:
            return EASE_QUARTIC_IN;
            break;
        case 10:
            return EASE_QUARTIC_OUT;
            break;
        case 11:
            return EASE_QUARTIC_IN_OUT;
            break;
        case 12:
            return EASE_QUARTIC_CENTER;
            break;
        case 13:
            return EASE_QUINTIC_IN;
            break;
        case 14:
            return EASE_QUINTIC_OUT;
            break;
        case 15:
            return EASE_QUINTIC_IN_OUT;
            break;
        case 16:
            return EASE_QUINTIC_CENTER;
            break;
        case 17:
            return EASE_SINUSOIDAL_IN;
            break;
        case 18:
            return EASE_SINUSOIDAL_OUT;
            break;
        case 19:
            return EASE_SINUSOIDAL_IN_OUT;
            break;
        case 20:
            return EASE_SINUSOIDAL_CENTER;
            break;
        case 21:
            return EASE_EXPONENTIAL_IN;
            break;
        case 22:
            return EASE_EXPONENTIAL_OUT;
            break;
        case 23:
            return EASE_EXPONENTIAL_IN_OUT;
            break;
        case 24:
            return EASE_EXPONENTIAL_CENTER;
            break;
        case 25:
            return EASE_CIRCULAR_IN;
            break;
        case 26:
            return EASE_CIRCULAR_OUT;
            break;
        case 27:
            return EASE_CIRCULAR_IN_OUT;
            break;
        case 28:
            return EASE_CIRCULAR_CENTER;
            break;
        case 29:
            return EASE_GAMMA;
            break;
        default:
            break;
    }
    return EASE_LINEAR;
}

fade_t val_to_fade(uint8_t val) {
    fade_t fade;

    if (val & 0x01) fade.FADED_HEAD = 1;
    if (val & 0x02) fade.FADED_TAIL = 1;

    return fade;
}

uint8_t fade_to_val(fade_t fade) {
    uint8_t val = 0;

    if (fade.FADED_HEAD) val |= 0x01;
    if (fade.FADED_TAIL) val |= 0x02;

    return val;
}

dir_t translate_dir(uint8_t val) {
    switch (val)
    {
        case 0:
            return DIR_BACKWARD;
            break;
        case 1:
            return DIR_FORWARD;
            break;
        default:
            break;
    }
    return DIR_BACKWARD;
}
