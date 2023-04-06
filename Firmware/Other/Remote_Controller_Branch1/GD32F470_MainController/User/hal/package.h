#ifndef _PACKAGE_H
#define _PACKAGE_H

#include <stdint.h>

#include "global.h"

#define PKG_BUF_MAX_LEN                     (32)
#define PKG_BUF_SIZE_INDEX                  (2)
#define PKG_BUF_SIZE(x)						(x[PKG_BUF_SIZE_INDEX])

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

#define PKG_FAIL                            (0x00)
#define PKG_OK                              (0x01)

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

void pack(uint8_t* buf, uint8_t cmd);
void init_configuration(void);

#endif
