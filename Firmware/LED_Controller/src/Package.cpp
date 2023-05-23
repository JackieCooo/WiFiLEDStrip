#include "Package.h"

bool Package::parse(uint8_t* buf, uint16_t size, package_t* pack) {
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
                pack->size = buf[i++];
                stage = 3;
                break;
            case 3:  // cmd
                pack->cmd = buf[i++];
                if (pack->cmd == PKG_CMD_WRITE_SETTING) {
                    stage = 4;
                }
                else if (pack->cmd == PKG_CMD_READ_SETTING) {
                    stage = 99;
                }
                else if (pack->cmd == PKG_CMD_ACK) {
                    stage = 99;
                }
                else if (pack->cmd == PKG_CMD_MATCH) {
                    stage = 5;
                }
                else {
                    return false;
                }
                break;
            case 4:  // write setting cmd
                pack->data.strip.power = buf[i++];
                pack->data.strip.mode = buf[i++];
                if (pack->data.strip.mode == PKG_MODE_NORMAL) {
                    stage = 23;
                }
                else if (pack->data.strip.mode == PKG_MODE_BREATHING) {
                    stage = 24;
                }
                else if (pack->data.strip.mode == PKG_MODE_LIGHTBEAM) {
                    stage = 25;
                }
                else if (pack->data.strip.mode == PKG_MODE_RAINBOW) {
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
                pack->data.ip.addr = PP_HTONL(LWIP_MAKEU32(a,b,c,d));
                stage = 99;
                break;
            case 23:  // write normal mode setting
                pack->data.strip.setting.normal.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // write breathing mode setting
                pack->data.strip.setting.breathing.color = PKG_CONCAT(buf[i++], buf[i++]);
                pack->data.strip.setting.breathing.duration = PKG_CONCAT(buf[i++], buf[i++]);
                pack->data.strip.setting.breathing.interval = PKG_CONCAT(buf[i++], buf[i++]);
                pack->data.strip.setting.breathing.ease = buf[i++];
                stage = 99;
                break;
            case 25:  // write lightbeam mode setting
                pack->data.strip.setting.lightbeam.color = PKG_CONCAT(buf[i++], buf[i++]);
                pack->data.strip.setting.lightbeam.len = buf[i++];
                pack->data.strip.setting.lightbeam.gap = buf[i++];
                pack->data.strip.setting.lightbeam.faded = buf[i++];
                pack->data.strip.setting.lightbeam.head = buf[i++];
                pack->data.strip.setting.lightbeam.tail = buf[i++];
                pack->data.strip.setting.lightbeam.dir = buf[i++];
                pack->data.strip.setting.lightbeam.speed = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 26:  // write rainbow mode setting
                pack->data.strip.setting.rainbow.speed = PKG_CONCAT(buf[i++], buf[i++]);
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

void Package::pack(uint8_t* buf, uint16_t size, package_t* pack) {
    uint8_t i = 0;

    buf[i++] = PKG_FRAME_HEAD1;
    buf[i++] = PKG_FRAME_HEAD2;
    buf[i++] = 0;  // fill it later
    if (pack->cmd == PKG_CMD_READ_REPLY) {
        buf[i++] = PKG_CMD_READ_REPLY;
        buf[i++] = pack->data.strip.power;
        buf[i++] = pack->data.strip.mode;
        if (pack->data.strip.mode == MODE_NORMAL) {
            uint16_t color = Rgb16Color(pack->data.strip.setting.normal.color).Color565;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
        }
        else if (pack->data.strip.mode == MODE_BREATHING) {
            uint16_t color = Rgb16Color(pack->data.strip.setting.normal.color).Color565;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = PKG_HIGH(pack->data.strip.setting.breathing.duration);
            buf[i++] = PKG_LOW(pack->data.strip.setting.breathing.duration);
            buf[i++] = PKG_HIGH(pack->data.strip.setting.breathing.interval);
            buf[i++] = PKG_LOW(pack->data.strip.setting.breathing.interval);
            buf[i++] = pack->data.strip.setting.breathing.ease;
        }
        else if (pack->data.strip.mode == MODE_LIGHTBEAM) {
            uint16_t color = Rgb16Color(pack->data.strip.setting.normal.color).Color565;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = pack->data.strip.setting.lightbeam.len;
            buf[i++] = pack->data.strip.setting.lightbeam.gap;
            buf[i++] = Package::parseFadedEnd(pack->data.strip.setting.lightbeam.faded);
            buf[i++] = pack->data.strip.setting.lightbeam.head;
            buf[i++] = pack->data.strip.setting.lightbeam.tail;
            buf[i++] = pack->data.strip.setting.lightbeam.dir;
            buf[i++] = PKG_HIGH(pack->data.strip.setting.lightbeam.speed);
            buf[i++] = PKG_LOW(pack->data.strip.setting.lightbeam.speed);
        }
        else if (pack->data.strip.mode == MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(pack->data.strip.setting.rainbow.speed);
            buf[i++] = PKG_LOW(pack->data.strip.setting.rainbow.speed);
        }
    }
    else if (pack->cmd == PKG_CMD_WRITE_REPLY) {
        buf[i++] = PKG_CMD_WRITE_REPLY;
        buf[i++] = PKG_REPLY_OK;
    }
    else if (pack->cmd == PKG_CMD_ACK_REPLY) {
        buf[i++] = PKG_CMD_ACK_REPLY;
        buf[i++] = PKG_REPLY_OK;
    }
    else if (pack->cmd == PKG_CMD_MATCH_REPLY) {
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
    if (pack->cmd == PKG_CMD_WRITE_SETTING) {
        Serial.println("Write setting cmd");
        pack->data.strip.power = pack->data.strip.power;
        pack->data.strip.mode = Package::packMode(pack->data.strip.mode);
        if (pack->data.strip.mode == PKG_MODE_NORMAL) {
            uint32_t rgb888 = Package::RGB565toRGB888(pack->data.strip.setting.normal.color);
            pack->data.strip.setting.normal.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
        }
        else if (pack->data.strip.mode == PKG_MODE_BREATHING) {
            uint32_t rgb888 = Package::RGB565toRGB888(pack->data.strip.setting.breathing.color);
            pack->data.strip.setting.breathing.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            pack->data.strip.setting.breathing.duration = pack->data.strip.setting.breathing.duration;
            pack->data.strip.setting.breathing.interval = pack->data.strip.setting.breathing.interval;
            pack->data.strip.setting.breathing.ease = Package::packEase(pack->data.strip.setting.breathing.ease);
        }
        else if (pack->data.strip.mode == PKG_MODE_LIGHTBEAM) {
            uint32_t rgb888 = Package::RGB565toRGB888(pack->data.strip.setting.lightbeam.color);
            pack->data.strip.setting.lightbeam.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            pack->data.strip.setting.lightbeam.gap = pack->data.strip.setting.lightbeam.gap;
            pack->data.strip.setting.lightbeam.len = pack->data.strip.setting.lightbeam.len;
            pack->data.strip.setting.lightbeam.faded_end = Package::packFadedEnd(pack->data.strip.setting.lightbeam.faded_end);
            pack->data.strip.setting.lightbeam.tail_len = pack->data.strip.setting.lightbeam.tail_len;
            pack->data.strip.setting.lightbeam.head_len = pack->data.strip.setting.lightbeam.head_len;
            pack->data.strip.setting.lightbeam.dir = Package::packDirection(pack->data.strip.setting.lightbeam.dir);
            pack->data.strip.setting.lightbeam.speed = pack->data.strip.setting.lightbeam.speed;
        }
        else if (pack->data.strip.mode == PKG_MODE_RAINBOW) {
            pack->data.strip.setting.rainbow.speed = pack->data.strip.setting.rainbow.speed;
        }
    }
}

IPAddress Package::parseTargetIP(void) {
    return IPAddress(pack->data.ip.addr);
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
