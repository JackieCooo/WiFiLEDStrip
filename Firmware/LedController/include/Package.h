#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "lwip/ip4_addr.h"

#include "NormalFeature.h"
#include "BreathingFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"
#include "RhythmFeature.h"
#include "StripHandler.h"
#include "global.h"

#define PKG_BUF_MAX_LEN                             (32)
#define PKG_BUF_SIZE_INDEX                          (2)

#define PKG_FRAME_HEAD1                             (0xAA)
#define PKG_FRAME_HEAD2                             (0xBB)
#define PKG_FRAME_TAIL1                             (0xEE)
#define PKG_FRAME_TAIL2                             (0xFF)

#define PKG_HIGH(x)                                 ((uint8_t)(x >> 8))
#define PKG_LOW(x)                                  ((uint8_t)x)
#define PKG_CONCAT(x, y)                            (((uint16_t)x) << 8 | y)
#define PKG_SET_DATA(PACK, MODE, INDEX, DATA)       (PACK.data.strip.setting.MODE.INDEX = DATA)
#define PKG_GET_DATA(PACK, MODE, INDEX, DATA)       (DATA = PACK.data.strip.setting.MODE.INDEX)
#define PKG_VAL(PACK, MODE, INDEX)                  (PACK.data.strip.setting.MODE.INDEX)
#define PKG_CHECK_CMD(PACK, CMD)                    (CMD == PACK.cmd)
#define PKG_CHECK_MODE(PACK, MODE)                  (MODE == PACK.data.strip.mode)

typedef enum {
    STAGE_FRAME_HEAD1,
    STAGE_FRAME_HEAD2,

    STAGE_SIZE_BYTE,
    STAGE_CMD_BYTE,

    STAGE_CMD_WRITE,
    STAGE_CMD_READ,
    STAGE_CMD_MATCH,
    STAGE_CMD_ACK,

    STAGE_WRITE_NORMAL,
    STAGE_WRITE_BREATHING,
    STAGE_WRITE_LIGHTBEAM,
    STAGE_WRITE_RAINBOW,
    STAGE_WRITE_RHYTHM,

    STAGE_FRAME_TAIL1,
    STAGE_FRAME_TAIL2,
} stage_enum;

typedef enum {
    PKG_OK,
    PKG_FAIL,
} pkg_err_t;

typedef enum {
    CMD_READ,
    CMD_WRITE,
    CMD_ACK,
    CMD_MATCH,
    CMD_READ_REPLY,
    CMD_WRITE_REPLY,
    CMD_ACK_REPLY,
    CMD_MATCH_REPLY,
} cmd_t;

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
                    uint8_t fade;
                    uint8_t head;
                    uint8_t tail;
                    uint8_t dir;
                    uint16_t speed;
                } lightbeam;
                struct {
                    uint16_t speed;
                } rainbow;
                struct {
                    uint16_t color;
                    uint8_t freq;
                } rhythm;
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
    static bool parse(const uint8_t* buf, uint16_t size, package_t& pack);
    static void pack(uint8_t* buf, uint16_t size, const package_t& pack);
    static void apply(const package_t& pack);
    static void accquire(package_t& pack, cmd_t cmd);
};
