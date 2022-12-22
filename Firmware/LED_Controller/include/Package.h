#pragma once

#include <Arduino.h>

typedef struct {
    uint8_t size;
    uint8_t mode;
    union {
        struct {
            uint16_t color;
        } normal_setting;
        struct {
            uint16_t color;
            uint16_t duration;
            uint16_t interval;
            uint8_t ease;
        } breathing_setting;
        struct {
            uint16_t color;
            uint16_t len;
            uint16_t interval;
            uint16_t head_len;
            uint16_t tail_len;
            uint16_t speed;
            uint8_t faded_end;
            uint8_t dir;
        } lightbeam_setting;
        struct {
            uint16_t speed;
        } rainbow_setting;
    } settings;
} package_t;

class Package {
public:
    void parse(void);
    void pack(void);

private:

};