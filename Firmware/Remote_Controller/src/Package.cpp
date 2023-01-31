#include <Package.h>

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
                if (_package.cmd == PKG_CMD_READ_REPLY) {
                    stage = 4;
                }
                else if (_package.cmd == PKG_CMD_WRITE_REPLY) {
                    stage = 5;
                }
                else if (_package.cmd == PKG_CMD_ACK_REPLY) {
                    stage = 6;
                }
                else {
                    return false;
                }
                break;
            case 4:  // read reply cmd
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
            case 5:  // write reply cmd
                _package.data.reply.resp = buf[i++];
                stage = 99;
                break;
            case 6:  // ack reply cmd
                uint8_t a, b, c, d;
                a = buf[i++];
                b = buf[i++];
                c = buf[i++];
                d = buf[i++];
                _package.data.ip.addr = PP_HTONL(LWIP_MAKEU32(a,b,c,d));
                stage = 99;
                break;
            case 23:  // read normal mode setting
                _package.data.strip.setting.normal.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // read breathing mode setting
                _package.data.strip.setting.breathing.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.duration = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.interval = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.breathing.ease = buf[i++];
                stage = 99;
                break;
            case 25:  // read lightbeam mode setting
                _package.data.strip.setting.lightbeam.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.gap = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.head_len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.tail_len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.speed = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.strip.setting.lightbeam.faded_end = buf[i++];
                _package.data.strip.setting.lightbeam.dir = buf[i++];
                stage = 99;
                break;
            case 26:  // read rainbow mode setting
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

void Package::pack(uint8_t* buf, uint8_t size) {
    uint8_t i = 0;

    buf[i++] = PKG_FRAME_HEAD1;
    buf[i++] = PKG_FRAME_HEAD2;
    buf[i++] = _calPackSize();
    if (_package.cmd == PKG_CMD_WRITE_SETTING) {
        buf[i++] = PKG_CMD_WRITE_SETTING;
        buf[i++] = _package.data.strip.power;
        buf[i++] = _package.data.strip.mode;
        if (_package.data.strip.mode == PKG_MODE_NORMAL) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.normal.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.normal.color);
        }
        else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.color);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.duration);
            buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.duration);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.interval);
            buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.interval);
            buf[i++] = _package.data.strip.setting.breathing.ease;
        }
        else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.color);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.len);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.len);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.gap);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.gap);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.head_len);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.head_len);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.tail_len);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.tail_len);
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.speed);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.speed);
            buf[i++] = _package.data.strip.setting.lightbeam.faded_end;
            buf[i++] = _package.data.strip.setting.lightbeam.dir;
        }
        else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.rainbow.speed);
            buf[i++] = PKG_LOW(_package.data.strip.setting.rainbow.speed);
        }
    }
    else if (_package.cmd == PKG_CMD_READ_SETTING) {
        buf[i++] = _package.cmd;
    }
    else if (_package.cmd == PKG_CMD_ACK) {
        buf[i++] = _package.cmd;
        buf[i++] = ip4_addr1_val(_package.data.ip);
        buf[i++] = ip4_addr2_val(_package.data.ip);
        buf[i++] = ip4_addr3_val(_package.data.ip);
        buf[i++] = ip4_addr4_val(_package.data.ip);
    }
    buf[i++] = PKG_FRAME_TAIL1;
    buf[i++] = PKG_FRAME_TAIL2;
}

package_t& Package::getPackage(void) {
    return _package;
}

uint8_t Package::_calPackSize(void) {
    uint8_t size = 6;  // frame head(2) + frame tail(2) + cmd(1) + size(1)
    if (_package.cmd == PKG_CMD_WRITE_SETTING) {
        size += 2;  // power(1) + mode(1)
        if (_package.data.strip.mode == PKG_MODE_NORMAL) {
            size += 2;
        }
        else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
            size += 7;
        }
        else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            size += 14;
        }
        else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
            size += 2;
        }
    }
    else if (_package.cmd == PKG_CMD_READ_SETTING) {
        
    }
    else if (_package.cmd == PKG_CMD_ACK) {
        size += 4;  // ip(4)
    }
    return size;
}

uint32_t Package::RGB565toRGB888(uint16_t& rgb565) {
    uint32_t rgb888;
    uint8_t r = (RGB565_R(rgb565) << 3) + (uint8_t)((rgb565 >> 11) & 0x07);
    uint8_t g = (RGB565_G(rgb565) << 2) + (uint8_t)((rgb565 >> 5) & 0x03);
    uint8_t b = (RGB565_B(rgb565) << 3) + (uint8_t)(rgb565 & 0x07);
    return rgb888 = CONCAT_RGB888(r, g, b);
}

uint16_t Package::RGB888toRGB565(uint32_t& rgb888) {
    uint16_t rgb565;
    uint8_t r = RGB888_R(rgb888) >> 3;
    uint8_t g = RGB888_G(rgb888) >> 2;
    uint8_t b = RGB888_B(rgb888) >> 3;
    return rgb565 = CONCAT_RGB565(r, g, b);
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
        case PKG_MOVE_RIGHT:
            return MOVE_RIGHT;
            break;
        case PKG_MOVE_LEFT:
            return MOVE_LEFT;
            break;
        default:
            break;
    }
    return MOVE_LEFT;
}

faded_end_t Package::packFadedEnd(uint8_t& value) {
    faded_end_t faded_end;
    if (value & PKG_FADED_HEAD) faded_end.FADED_HEAD = 1;
    if (value & PKG_FADED_TAIL) faded_end.FADED_TAIL = 1;
    return faded_end;
}

uint8_t Package::parseFadedEnd(faded_end_t& value) {
    uint8_t faded_end = 0;
    if (value.FADED_HEAD) faded_end |= PKG_FADED_HEAD;
    if (value.FADED_TAIL) faded_end |= PKG_FADED_TAIL;
    return faded_end;
}
