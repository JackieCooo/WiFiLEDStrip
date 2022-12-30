#pragma once

#include <Arduino.h>

#include "global.h"

#define PKG_BUF_MAX_LEN                     (32)

#define PKG_FRAME_HEAD1                     (0xAA)
#define PKG_FRAME_HEAD2                     (0xBB)
#define PKG_FRAME_TAIL1                     (0xEE)
#define PKG_FRAME_TAIL2                     (0xFF)

#define PKG_CMD_READ_SETTING                (0x01)
#define PKG_CMD_WRITE_SETTING               (0x02)
#define PKG_CMD_ACK                         (0x03)
#define PKG_CMD_READ_REPLY                  (0x04)
#define PKG_CMD_WRITE_REPLY                 (0x05)
#define PKG_CMD_ACK_REPLY                   (0x06)

#define PKG_REPLY_FAIL                      (0x00)
#define PKG_REPLY_OK                        (0x01)

#define PKG_MODE_NORMAL                     (0x00)
#define PKG_MODE_BREATHING                  (0x01)
#define PKG_MODE_LIGHTBEAM                  (0x02)
#define PKG_MODE_RAINBOW                    (0x03)

#define PKG_FADED_DISABLE                   (0)
#define PKG_FADED_HEAD                      (1 << 0)
#define PKG_FADED_TAIL                      (1 << 1)

#define PKG_MOVE_RIGHT                      (0x00)
#define PKG_MOVE_LEFT                       (0x01)

#define PKG_EASE_LINEAR                     (0x00)
#define PKG_EASE_QUADRATIC_IN               (0x01)
#define PKG_EASE_QUADRATIC_OUT              (0x02)
#define PKG_EASE_QUADRATIC_IN_OUT           (0x03)
#define PKG_EASE_QUADRATIC_CENTER           (0x04)
#define PKG_EASE_CUBIC_IN                   (0x05)
#define PKG_EASE_CUBIC_OUT                  (0x06)
#define PKG_EASE_CUBIC_IN_OUT               (0x07)
#define PKG_EASE_CUBIC_CENTER               (0x08)
#define PKG_EASE_QUARTIC_IN                 (0x09)
#define PKG_EASE_QUARTIC_OUT                (0x0A)
#define PKG_EASE_QUARTIC_IN_OUT             (0x0B)
#define PKG_EASE_QUARTIC_CENTER             (0x0C)
#define PKG_EASE_QUINTIC_IN                 (0x0D)
#define PKG_EASE_QUINTIC_OUT                (0x0E)
#define PKG_EASE_QUINTIC_IN_OUT             (0x0F)
#define PKG_EASE_QUINTIC_CENTER             (0x10)
#define PKG_EASE_SINUSOIDAL_IN              (0x11)
#define PKG_EASE_SINUSOIDAL_OUT             (0x12)
#define PKG_EASE_SINUSOIDAL_IN_OUT          (0x13)
#define PKG_EASE_SINUSOIDAL_CENTER          (0x14)
#define PKG_EASE_EXPONENTIAL_IN             (0x15)
#define PKG_EASE_EXPONENTIAL_OUT            (0x16)
#define PKG_EASE_EXPONENTIAL_IN_OUT         (0x17)
#define PKG_EASE_EXPONENTIAL_CENTER         (0x18)
#define PKG_EASE_CIRCULAR_IN                (0x19)
#define PKG_EASE_CIRCULAR_OUT               (0x1A)
#define PKG_EASE_CIRCULAR_IN_OUT            (0x1B)
#define PKG_EASE_CIRCULAR_CENTER            (0x1C)
#define PKG_EASE_GAMMA                      (0x1D)

#define PKG_DATA_ALL                        (0x00)
#define PKG_DATA_COLOR                      (0x01)
#define PKG_DATA_DURATION                   (0x02)
#define PKG_DATA_INTERVAL                   (0x03)
#define PKG_DATA_SPEED                      (0x04)
#define PKG_DATA_LEN                        (0x05)
#define PKG_DATA_FADED_END                  (0x06)
#define PKG_DATA_TAIL_LEN                   (0x07)
#define PKG_DATA_HEAD_LEN                   (0x08)
#define PKG_DATA_DIRECTION                  (0x09)
#define PKG_DATA_EASE                       (0x0A)
#define PKG_DATA_MODE                       (0x0B)

#define PKG_HIGH(x)                         ((uint8_t)(x >> 8))
#define PKG_LOW(x)                          ((uint8_t)x)
#define PKG_CONCAT(x, y)                    (((uint16_t)x) << 8 | y)

#define RGB565_R(x)                         ((uint8_t)((x & 0xF800) >> 11))
#define RGB565_G(x)                         ((uint8_t)((x & 0x07E0) >> 5))
#define RGB565_B(x)                         ((uint8_t)((x & 0x001F) >> 0))
#define CONCAT_RGB565(r, g, b)              (((uint16_t)r << 11) + ((uint16_t)g << 5) + (uint16_t)b)

#define RGB888_R(x)                         ((uint8_t)((x & 0xFF0000) >> 16))
#define RGB888_G(x)                         ((uint8_t)((x & 0x00FF00) >> 8))
#define RGB888_B(x)                         ((uint8_t)((x & 0x0000FF) >> 0))
#define CONCAT_RGB888(r, g, b)              (((uint32_t)r << 16) + ((uint32_t)g << 8) + (uint32_t)b)

#define INADDR_1(x)                         ((uint8_t)((x & 0xFF000000) >> 24))
#define INADDR_2(x)                         ((uint8_t)((x & 0x00FF0000) >> 16))
#define INADDR_3(x)                         ((uint8_t)((x & 0x0000FF00) >> 8))
#define INADDR_4(x)                         ((uint8_t)((x & 0x000000FF) >> 0))
#define CONCAT_INADDR(a, b, c, d)           (((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)c << 8) + (uint32_t)d)

typedef struct {
    uint8_t size;
    uint8_t cmd;
    union {
        struct {
            uint8_t subcmd;
            uint8_t mode;
            setting_data_t setting;
        } strip;
        struct {
            uint8_t resp;
        } resp;
        struct {
            uint8_t a;
            uint8_t b;
            uint8_t c;
            uint8_t d;
        } ip;
    } data;
} package_t;

class Package {
public:
    bool parse(uint8_t* buf, uint8_t size);
    void pack(uint8_t* buf, uint8_t size);
    package_t& getPackage(void);
    static uint32_t RGB565toRGB888(uint16_t& rgb565);
    static uint16_t RGB888toRGB565(uint32_t& rgb888);

private:
    package_t _package;

    uint8_t _calPackSize(void);
};