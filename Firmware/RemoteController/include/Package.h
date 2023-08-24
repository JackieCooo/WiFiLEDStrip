#pragma once

#include <Arduino.h>
#include <WiFi.h>

#include "lwip/ip4_addr.h"

#include "global.h"
#include "config.h"

#define PKG_BUF_MAX_LEN                     (32)
#define PKG_BUF_SIZE_INDEX                  (2)

#define PKG_FRAME_HEAD1                     (0xAA)
#define PKG_FRAME_HEAD2                     (0xBB)
#define PKG_FRAME_TAIL1                     (0xEE)
#define PKG_FRAME_TAIL2                     (0xFF)

#define PKG_HIGH(x)                         ((uint8_t)(x >> 8))
#define PKG_LOW(x)                          ((uint8_t)x)
#define PKG_CONCAT(x, y)                    (((uint16_t)x) << 8 | y)

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
    static bool parse(uint8_t* buf, uint16_t size, package_t& pack);
    static void pack(uint8_t* buf, uint16_t size, const package_t& pack);
    static void apply(const package_t& pack);
    static void accquire(package_t& pack, cmd_t cmd);
};
