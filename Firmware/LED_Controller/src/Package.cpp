#include "Package.h"

bool Package::parse(uint8_t* buf, uint8_t size) {
    uint8_t stage = 0;
    uint8_t i = 0;
    for (; i < size;) {
        switch (stage)
        {
            case 0:  // frame head 1
                if (buf[i++] == PKG_FRAME_HEAD1) {
                    stage = 1;
                }
                else return false;
                break;
            case 1:  // frame head 2
                if (buf[i++] == PKG_FRAME_HEAD2) {
                    stage = 2;
                }
                else return false;
                break;
            case 2:  // frame size
                _package.size = buf[i++];
                stage = 3;
                break;
            case 3:  // cmd
                _package.cmd = buf[i++];
                if (_package.cmd == PKG_CMD_WRITE_SETTING) {
                    stage = 4;
                }
                else if (_package.cmd == PKG_CMD_READ_SETTING) {
                    stage = 99;
                }
                else if (_package.cmd == PKG_CMD_ACK) {
                    stage = 99;
                }
                else if (_package.cmd == PKG_CMD_MATCH) {
                    stage = 5;
                }
                else {
                    return false;
                }
                break;
            case 4:  // write setting cmd
                _package.data.strip.power = buf[i++];
                _package.data.strip.mode = buf[i++];
                if (_package.data.strip.mode == PKG_MODE_NORMAL) {
                    stage = 23;
                }
                else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
                    stage = 24;
                }
                else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
                    stage = 25;
                }
                else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
                    stage = 26;
                }
                else {
                    return false;
                }
                break;
            case 5:
                uint8_t a, b, c, d;
                a = buf[i++];
                b = buf[i++];
                c = buf[i++];
                d = buf[i++];
                _package.data.ip.addr = PP_HTONL(LWIP_MAKEU32(a,b,c,d));
                stage = 99;
                break;
            case 23:  // write normal mode setting
                _package.data.strip.setting.normal.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // write breathing mode setting
                _package.data.strip.setting.breathing.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.duration = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.interval = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.ease = buf[i++];
                stage = 99;
                break;
            case 25:  // write lightbeam mode setting
                _package.data.strip.setting.lightbeam.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.len = buf[i++];
                _package.data.strip.setting.lightbeam.gap = buf[i++];
                _package.data.strip.setting.lightbeam.faded_end = buf[i++];
                _package.data.strip.setting.lightbeam.head_len = buf[i++];
                _package.data.strip.setting.lightbeam.tail_len = buf[i++];
                _package.data.strip.setting.lightbeam.dir = buf[i++];
                _package.data.strip.setting.lightbeam.speed = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 26:  // write rainbow mode setting
                _package.data.strip.setting.rainbow.speed = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 99:  // frame tail 1
                if (buf[i++] == PKG_FRAME_TAIL1) stage = 100;
                else return false;
                break;
            case 100:  // frmae tail 2
                if (buf[i++] == PKG_FRAME_TAIL2) return true;
                else return false;
                break;
            default:
                break;
        }
    }
    return false;
}

void Package::pack(uint8_t* buf, uint8_t cmd) {
    uint8_t i = 0;

    buf[i++] = PKG_FRAME_HEAD1;
    buf[i++] = PKG_FRAME_HEAD2;
    buf[i++] = 0;  // fill it later
    if (cmd == PKG_CMD_READ_REPLY) {
        buf[i++] = PKG_CMD_READ_REPLY;
        buf[i++] = configuration.power;
        buf[i++] = configuration.mode;
        if (configuration.mode == MODE_NORMAL) {
            uint16_t color = RGB888toRGB565(CONCAT_RGB888(configuration.setting.normal.color.R, configuration.setting.normal.color.G, configuration.setting.normal.color.B));
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
        }
        else if (configuration.mode == MODE_BREATHING) {
            uint16_t color = RGB888toRGB565(CONCAT_RGB888(configuration.setting.breathing.color.R, configuration.setting.breathing.color.G, configuration.setting.breathing.color.B));
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = PKG_HIGH(configuration.setting.breathing.duration);
            buf[i++] = PKG_LOW(configuration.setting.breathing.duration);
            buf[i++] = PKG_HIGH(configuration.setting.breathing.interval);
            buf[i++] = PKG_LOW(configuration.setting.breathing.interval);
            buf[i++] = configuration.setting.breathing.ease;
        }
        else if (configuration.mode == MODE_LIGHTBEAM) {
            uint16_t color = RGB888toRGB565(CONCAT_RGB888(configuration.setting.lightbeam.color.R, configuration.setting.lightbeam.color.G, configuration.setting.lightbeam.color.B));
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = configuration.setting.lightbeam.len;
            buf[i++] = configuration.setting.lightbeam.gap;
            buf[i++] = Package::parseFadedEnd(configuration.setting.lightbeam.faded_end);
            buf[i++] = configuration.setting.lightbeam.head_len;
            buf[i++] = configuration.setting.lightbeam.tail_len;
            buf[i++] = configuration.setting.lightbeam.dir;
            buf[i++] = PKG_HIGH(configuration.setting.lightbeam.speed);
            buf[i++] = PKG_LOW(configuration.setting.lightbeam.speed);
        }
        else if (configuration.mode == MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(configuration.setting.rainbow.speed);
            buf[i++] = PKG_LOW(configuration.setting.rainbow.speed);
        }
    }
    else if (cmd == PKG_CMD_WRITE_REPLY) {
        buf[i++] = PKG_CMD_WRITE_REPLY;
        buf[i++] = PKG_REPLY_OK;
    }
    else if (cmd == PKG_CMD_ACK_REPLY) {
        buf[i++] = PKG_CMD_ACK_REPLY;
        buf[i++] = PKG_REPLY_OK;
    }
    else if (cmd == PKG_CMD_MATCH_REPLY) {
        buf[i++] = PKG_CMD_MATCH_REPLY;
        IPAddress ip = WiFi.localIP();
        buf[i++] = ip[0];
        buf[i++] = ip[1];
        buf[i++] = ip[2];
        buf[i++] = ip[3];
    }
    buf[i++] = PKG_FRAME_TAIL1;
    buf[i++] = PKG_FRAME_TAIL2;

    buf[PKG_BUF_SIZE_INDEX] = i;  // package size
}

void Package::parseFromPackage(void) {
    if (_package.cmd == PKG_CMD_WRITE_SETTING) {
        Serial.println("Write setting cmd");
        configuration.power = _package.data.strip.power;
        configuration.mode = Package::packMode(_package.data.strip.mode);
        if (_package.data.strip.mode == PKG_MODE_NORMAL) {
            uint32_t rgb888 = Package::RGB565toRGB888(_package.data.strip.setting.normal.color);
            configuration.setting.normal.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
        }
        else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
            uint32_t rgb888 = Package::RGB565toRGB888(_package.data.strip.setting.breathing.color);
            configuration.setting.breathing.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            configuration.setting.breathing.duration = _package.data.strip.setting.breathing.duration;
            configuration.setting.breathing.interval = _package.data.strip.setting.breathing.interval;
            configuration.setting.breathing.ease = Package::packEase(_package.data.strip.setting.breathing.ease);
        }
        else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            uint32_t rgb888 = Package::RGB565toRGB888(_package.data.strip.setting.lightbeam.color);
            configuration.setting.lightbeam.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            configuration.setting.lightbeam.gap = _package.data.strip.setting.lightbeam.gap;
            configuration.setting.lightbeam.len = _package.data.strip.setting.lightbeam.len;
            configuration.setting.lightbeam.faded_end = Package::packFadedEnd(_package.data.strip.setting.lightbeam.faded_end);
            configuration.setting.lightbeam.tail_len = _package.data.strip.setting.lightbeam.tail_len;
            configuration.setting.lightbeam.head_len = _package.data.strip.setting.lightbeam.head_len;
            configuration.setting.lightbeam.dir = Package::packDirection(_package.data.strip.setting.lightbeam.dir);
            configuration.setting.lightbeam.speed = _package.data.strip.setting.lightbeam.speed;
        }
        else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
            configuration.setting.rainbow.speed = _package.data.strip.setting.rainbow.speed;
        }
    }
}

IPAddress Package::parseTargetIP(void) {
    return IPAddress(_package.data.ip.addr);
}

package_t& Package::getPackage(void) {
    return _package;
}

uint32_t Package::RGB565toRGB888(uint16_t rgb565) {
    uint8_t r = (RGB565_R(rgb565) << 3) + (uint8_t)((rgb565 >> 11) & 0x07);
    uint8_t g = (RGB565_G(rgb565) << 2) + (uint8_t)((rgb565 >> 5) & 0x03);
    uint8_t b = (RGB565_B(rgb565) << 3) + (uint8_t)(rgb565 & 0x07);
    return CONCAT_RGB888(r, g, b);
}

uint16_t Package::RGB888toRGB565(uint32_t rgb888) {
    uint8_t r = RGB888_R(rgb888) >> 3;
    uint8_t g = RGB888_G(rgb888) >> 2;
    uint8_t b = RGB888_B(rgb888) >> 3;
    return CONCAT_RGB565(r, g, b);
}

ease_t Package::packEase(uint8_t& ease) {
    switch (ease)
    {
        case PKG_EASE_LINEAR:
            return EASE_LINEAR;
            break;
        case PKG_EASE_QUADRATIC_IN:
            return EASE_QUADRATIC_IN;
            break;
        case PKG_EASE_QUADRATIC_OUT:
            return EASE_QUADRATIC_OUT;
            break;
        case PKG_EASE_QUADRATIC_IN_OUT:
            return EASE_QUADRATIC_IN_OUT;
            break;
        case PKG_EASE_QUADRATIC_CENTER:
            return EASE_QUADRATIC_CENTER;
            break;
        case PKG_EASE_CUBIC_IN:
            return EASE_CUBIC_IN;
            break;
        case PKG_EASE_CUBIC_OUT:
            return EASE_CUBIC_OUT;
            break;
        case PKG_EASE_CUBIC_IN_OUT:
            return EASE_CUBIC_IN_OUT;
            break;
        case PKG_EASE_CUBIC_CENTER:
            return EASE_CUBIC_CENTER;
            break;
        case PKG_EASE_QUARTIC_IN:
            return EASE_QUARTIC_IN;
            break;
        case PKG_EASE_QUARTIC_OUT:
            return EASE_QUARTIC_OUT;
            break;
        case PKG_EASE_QUARTIC_IN_OUT:
            return EASE_QUARTIC_IN_OUT;
            break;
        case PKG_EASE_QUARTIC_CENTER:
            return EASE_QUARTIC_CENTER;
            break;
        case PKG_EASE_QUINTIC_IN:
            return EASE_QUINTIC_IN;
            break;
        case PKG_EASE_QUINTIC_OUT:
            return EASE_QUINTIC_OUT;
            break;
        case PKG_EASE_QUINTIC_IN_OUT:
            return EASE_QUINTIC_IN_OUT;
            break;
        case PKG_EASE_QUINTIC_CENTER:
            return EASE_QUINTIC_CENTER;
            break;
        case PKG_EASE_SINUSOIDAL_IN:
            return EASE_SINUSOIDAL_IN;
            break;
        case PKG_EASE_SINUSOIDAL_OUT:
            return EASE_SINUSOIDAL_OUT;
            break;
        case PKG_EASE_SINUSOIDAL_IN_OUT:
            return EASE_SINUSOIDAL_IN_OUT;
            break;
        case PKG_EASE_SINUSOIDAL_CENTER:
            return EASE_SINUSOIDAL_CENTER;
            break;
        case PKG_EASE_EXPONENTIAL_IN:
            return EASE_EXPONENTIAL_IN;
            break;
        case PKG_EASE_EXPONENTIAL_OUT:
            return EASE_EXPONENTIAL_OUT;
            break;
        case PKG_EASE_EXPONENTIAL_IN_OUT:
            return EASE_EXPONENTIAL_IN_OUT;
            break;
        case PKG_EASE_EXPONENTIAL_CENTER:
            return EASE_EXPONENTIAL_CENTER;
            break;
        case PKG_EASE_CIRCULAR_IN:
            return EASE_CIRCULAR_IN;
            break;
        case PKG_EASE_CIRCULAR_OUT:
            return EASE_CIRCULAR_OUT;
            break;
        case PKG_EASE_CIRCULAR_IN_OUT:
            return EASE_CIRCULAR_IN_OUT;
            break;
        case PKG_EASE_CIRCULAR_CENTER:
            return EASE_CIRCULAR_CENTER;
            break;
        case PKG_EASE_GAMMA:
            return EASE_GAMMA;
            break;
        default:
            break;
    }
    return EASE_LINEAR;
}

dir_t Package::packDirection(uint8_t& dir) {
    switch (dir)
    {
        case PKG_MOVE_LEFT:
            return MOVE_LEFT;
            break;
        case PKG_MOVE_RIGHT:
            return MOVE_RIGHT;
            break;
        default:
            break;
    }
    return MOVE_LEFT;
}

led_mode_t Package::packMode(uint8_t& mode) {
    switch (mode)
    {
        case PKG_MODE_NORMAL:
            return MODE_NORMAL;
            break;
        case PKG_MODE_BREATHING:
            return MODE_BREATHING;
            break;
        case PKG_MODE_LIGHTBEAM:
            return MODE_LIGHTBEAM;
            break;
        case PKG_MODE_RAINBOW:
            return MODE_RAINBOW;
            break;
        default:
            break;
    }
    return MODE_NORMAL;
}

faded_end_t Package::packFadedEnd(uint8_t& value) {
    faded_end_t faded_end;
    if (value & PKG_FADED_HEAD) {
        faded_end.FADED_HEAD = 1;
    }
    if (value & PKG_FADED_TAIL) {
        faded_end.FADED_TAIL = 1;
    }
    return faded_end;
}

uint8_t Package::parseFadedEnd(faded_end_t& value) {
    uint8_t faded_end = 0;
    if (value.FADED_HEAD) {
        faded_end |= PKG_FADED_HEAD;
    }
    if (value.FADED_TAIL) {
        faded_end |= PKG_FADED_TAIL;
    }
    return faded_end;
}
