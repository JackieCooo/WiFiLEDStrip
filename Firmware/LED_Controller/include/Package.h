#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "NormalFeature.h"
#include "BreathingFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"
#include "StripHandler.h"
#include "global.h"

#define PKG_BUF_MAX_LEN                     (32)
#define PKG_BUF_SIZE_INDEX                  (2)

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
#define PKG_CMD_MATCH                       (0x07)
#define PKG_CMD_MATCH_REPLY                 (0x08)

#define PKG_REPLY_FAIL                      (0x00)
#define PKG_REPLY_OK                        (0x01)

#define PKG_MODE_NORMAL                     (0x00)
#define PKG_MODE_BREATHING                  (0x01)
#define PKG_MODE_LIGHTBEAM                  (0x02)
#define PKG_MODE_RAINBOW                    (0x03)

#define PKG_FADED_HEAD                      (1 << 0)
#define PKG_FADED_TAIL                      (1 << 1)

#define PKG_MOVE_LEFT                       (0x00)
#define PKG_MOVE_RIGHT                      (0x01)

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

#define PKG_HIGH(x)                         ((uint8_t)(x >> 8))
#define PKG_LOW(x)                          ((uint8_t)x)
#define PKG_CONCAT(x, y)                    (((uint16_t)x) << 8 | y)

typedef struct {
    uint8_t size;
    uint8_t cmd;
    union {
        struct {
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
                    uint8_t len;
                    uint8_t gap;
                    uint8_t faded;
                    uint8_t head;
                    uint8_t tail;
                    uint8_t dir;
                    uint16_t speed;
                } lightbeam;
                struct {
                    uint16_t speed;
                } rainbow;
            } setting;
        } strip;
        struct {
            uint8_t resp;
        } reply;
        ip4_addr_t ip;
    } data;
} package_t;

class Package {
public:
    static bool parse(uint8_t* buf, uint16_t size, package_t* pack);
    static void pack(uint8_t* buf, uint16_t size, package_t* pack);
    void parseFromPackage(void);
    IPAddress parseTargetIP(void);

    static faded_end_t packFadedEnd(uint8_t& value);
    static uint8_t parseFadedEnd(faded_end_t& value);
};
