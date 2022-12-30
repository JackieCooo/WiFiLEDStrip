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
                _package.data.strip.subcmd = buf[i++];
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
                _package.data.resp.resp = buf[i++];
                stage = 99;
                break;
            case 6:  // ack reply cmd
                _package.data.ip.a = buf[i++];
                _package.data.ip.b = buf[i++];
                _package.data.ip.c = buf[i++];
                _package.data.ip.d = buf[i++];
                stage = 99;
                break;
            case 23:  // read normal mode setting
                _package.data.strip.setting.normal.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // read breathing mode setting
                _package.data.strip.setting.breathing.color = PKG_CONCAT(buf[i++], buf[i++]);
                if (_package.data.strip.subcmd != PKG_DATA_COLOR) {
                    _package.data.strip.setting.breathing.duration = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.breathing.interval = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.breathing.ease = buf[i++];
                }
                stage = 99;
                break;
            case 25:  // read lightbeam mode setting
                _package.data.strip.setting.lightbeam.color = PKG_CONCAT(buf[i++], buf[i++]);
                if (_package.data.strip.subcmd != PKG_DATA_COLOR) {
                    _package.data.strip.setting.lightbeam.len = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.lightbeam.interval = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.lightbeam.head_len = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.lightbeam.tail_len = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.lightbeam.speed = PKG_CONCAT(buf[i++], buf[i++]);
                    _package.data.strip.setting.lightbeam.faded_end = buf[i++];
                    _package.data.strip.setting.lightbeam.dir = buf[i++];
                }
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
        buf[i++] = _package.data.strip.subcmd;
        buf[i++] = _package.data.strip.mode;
        if (_package.data.strip.mode == PKG_MODE_NORMAL) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.normal.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.normal.color);
        }
        else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.color);
            if (_package.data.strip.subcmd != PKG_DATA_COLOR) {
                buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.duration);
                buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.duration);
                buf[i++] = PKG_HIGH(_package.data.strip.setting.breathing.interval);
                buf[i++] = PKG_LOW(_package.data.strip.setting.breathing.interval);
                buf[i++] = _package.data.strip.setting.breathing.ease;
            }
        }
        else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.color);
            buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.color);
            if (_package.data.strip.subcmd != PKG_DATA_COLOR) {
                buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.len);
                buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.len);
                buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.interval);
                buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.interval);
                buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.head_len);
                buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.head_len);
                buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.tail_len);
                buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.tail_len);
                buf[i++] = PKG_HIGH(_package.data.strip.setting.lightbeam.speed);
                buf[i++] = PKG_LOW(_package.data.strip.setting.lightbeam.speed);
                buf[i++] = _package.data.strip.setting.lightbeam.faded_end;
                buf[i++] = _package.data.strip.setting.lightbeam.dir;
            }
        }
        else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(_package.data.strip.setting.rainbow.speed);
            buf[i++] = PKG_LOW(_package.data.strip.setting.rainbow.speed);
        }
    }
    else if (_package.cmd == PKG_CMD_READ_SETTING) {
        buf[i++] = _package.cmd;
        buf[i++] = _package.data.strip.subcmd;
    }
    else if (_package.cmd == PKG_CMD_ACK) {
        buf[i++] = _package.cmd;
        buf[i++] = _package.data.ip.a;
        buf[i++] = _package.data.ip.b;
        buf[i++] = _package.data.ip.c;
        buf[i++] = _package.data.ip.d;
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
        size += 2;  // subcmd(1) + mode(1)
        if (_package.data.strip.mode == PKG_MODE_NORMAL) {
            size += 2;
        }
        else if (_package.data.strip.mode == PKG_MODE_BREATHING) {
            if (_package.data.strip.subcmd == PKG_DATA_COLOR) size += 2;
            else size += 7;
        }
        else if (_package.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            if (_package.data.strip.subcmd == PKG_DATA_COLOR) size += 2;
            else size += 14;
        }
        else if (_package.data.strip.mode == PKG_MODE_RAINBOW) {
            size += 2;
        }
    }
    else if (_package.cmd == PKG_CMD_READ_SETTING) {
        size += 1;  // subcmd(1)
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
