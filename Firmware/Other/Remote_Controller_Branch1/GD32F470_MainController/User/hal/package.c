#include "hal/package.h"

static uint8_t parseFadedEnd(faded_end_t value);

void pack(uint8_t* buf, uint8_t cmd) {
    uint8_t i = 0;

    buf[i++] = PKG_FRAME_HEAD1;
    buf[i++] = PKG_FRAME_HEAD2;
    buf[i++] = 0;  // fill it later
    if (cmd == PKG_CMD_WRITE_SETTING) {
        buf[i++] = PKG_CMD_WRITE_SETTING;
        buf[i++] = configuration.power;
        buf[i++] = configuration.mode;
        if (configuration.mode == PKG_MODE_NORMAL) {
            uint16_t color = lv_color_to16(configuration.setting.normal.color);
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
        }
        else if (configuration.mode == PKG_MODE_BREATHING) {
            uint16_t color = lv_color_to16(configuration.setting.breathing.color);
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = PKG_HIGH(configuration.setting.breathing.duration);
            buf[i++] = PKG_LOW(configuration.setting.breathing.duration);
            buf[i++] = PKG_HIGH(configuration.setting.breathing.interval);
            buf[i++] = PKG_LOW(configuration.setting.breathing.interval);
            buf[i++] = configuration.setting.breathing.ease;
        }
        else if (configuration.mode == PKG_MODE_LIGHTBEAM) {
            uint16_t color = lv_color_to16(configuration.setting.lightbeam.color);
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = configuration.setting.lightbeam.len;
            buf[i++] = configuration.setting.lightbeam.gap;
            buf[i++] = parseFadedEnd(configuration.setting.lightbeam.faded_end);
            buf[i++] = configuration.setting.lightbeam.head_len;
            buf[i++] = configuration.setting.lightbeam.tail_len;
            buf[i++] = configuration.setting.lightbeam.dir;
            buf[i++] = PKG_HIGH(configuration.setting.lightbeam.speed);
            buf[i++] = PKG_LOW(configuration.setting.lightbeam.speed);
        }
        else if (configuration.mode == PKG_MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(configuration.setting.rainbow.speed);
            buf[i++] = PKG_LOW(configuration.setting.rainbow.speed);
        }
    }

    buf[i++] = PKG_FRAME_TAIL1;
    buf[i++] = PKG_FRAME_TAIL2;

    buf[PKG_BUF_SIZE_INDEX] = i - 4;  // package size
}

uint8_t parseFadedEnd(faded_end_t value) {
    uint8_t faded_end = 0;
    if (value.FADED_HEAD) faded_end |= PKG_FADED_HEAD;
    if (value.FADED_TAIL) faded_end |= PKG_FADED_TAIL;
    return faded_end;
}
