#pragma once

#include <Arduino.h>

#define PKG_CMD_READ_SETTING                (0x01)
#define PKG_CMD_WRITE_SETTING               (0x02)
#define PKG_CMD_ACK                         (0x03)

#define PKG_MODE_NORMAL                     (0x00)
#define PKG_MODE_BREATHING                  (0x01)
#define PKG_MODE_LIGHTBEAM                  (0x02)
#define PKG_MODE_RAINBOW                    (0x03)

#define PKG_FADED_DISABLE                   (0)
#define PKG_FADED_HEAD                      (1 << 0)
#define PKG_FADED_TAIL                      (1 << 1)

#define PKG_MOVE_LEFT                       (0x00)
#define PKG_MOVE_RIGHT                      (0x01)

#define PKG_HIGH(x)                         ((uint8_t)(x >> 8))
#define PKG_LOW(x)                          ((uint8_t)x)
#define PKG_CONCAT(x, y)                    (((uint16_t)x) << 8 | y)

typedef struct {
    uint8_t size;
    uint8_t cmd;
    struct {
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
    } data;
} package_t;

class Package {
public:
    bool parse(uint8_t* buf, uint8_t size);
    bool pack(uint8_t* buf, uint8_t size);
    package_t& getPackage(void);

private:
    package_t _package;

    uint8_t _calPackSize(void);
};