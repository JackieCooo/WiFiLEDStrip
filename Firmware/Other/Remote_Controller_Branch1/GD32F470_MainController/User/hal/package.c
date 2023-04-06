#include "hal/package.h"

configuration_t configuration;

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

void init_configuration(void)
{
	configuration.mode = DEFAULT_MODE;
	configuration.power = DEFAULT_POWER;
	
	configuration.setting.normal.color = DEFAULT_COLOR;
	
	configuration.setting.breathing.color = DEFAULT_COLOR;
	configuration.setting.breathing.duration = DEFAULT_DURATION;
	configuration.setting.breathing.interval = DEFAULT_INTERVAL;
	configuration.setting.breathing.ease = DEFAULT_EASE;
	
	configuration.setting.lightbeam.color = DEFAULT_COLOR;
	configuration.setting.lightbeam.len = DEFAULT_LEN;
	configuration.setting.lightbeam.gap = DEFAULT_GAP;
	if (DEFAULT_FADED_END & PKG_FADED_HEAD) configuration.setting.lightbeam.faded_end.FADED_HEAD = 1;
	if (DEFAULT_FADED_END & PKG_FADED_TAIL) configuration.setting.lightbeam.faded_end.FADED_TAIL = 1;
	configuration.setting.lightbeam.head_len = DEFAULT_HEAD_LEN;
	configuration.setting.lightbeam.tail_len = DEFAULT_TAIL_LEN;
	configuration.setting.lightbeam.speed = DEFAULT_SPEED;
	configuration.setting.lightbeam.dir = DEFAULT_DIR;
	
	configuration.setting.rainbow.speed = DEFAULT_SPEED;
}

static uint8_t parseFadedEnd(faded_end_t value) {
    uint8_t faded_end = 0;
    if (value.FADED_HEAD) faded_end |= PKG_FADED_HEAD;
    if (value.FADED_TAIL) faded_end |= PKG_FADED_TAIL;
    return faded_end;
}
