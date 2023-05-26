#include "Package.h"

bool Package::parse(uint8_t* buf, uint16_t size, package_t& pack) {
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
                pack.size = buf[i++];
                stage = 3;
                break;
            case 3:  // cmd
                pack.cmd = buf[i++];
                if (pack.cmd == CMD_READ_REPLY) {
                    stage = 4;
                }
                else if (pack.cmd == CMD_WRITE_REPLY) {
                    stage = 5;
                }
                else if (pack.cmd == CMD_ACK_REPLY) {
                    stage = 6;
                }
                else if (pack.cmd == CMD_MATCH_REPLY) {
                    stage = 7;
                }
                else {
                    return false;
                }
                break;
            case 4:  // read reply cmd
                pack.data.strip.power = buf[i++];
                pack.data.strip.mode = buf[i++];
                if (pack.data.strip.mode == MODE_NORMAL) {
                    stage = 23;
                }
                else if (pack.data.strip.mode == MODE_BREATHING) {
                    stage = 24;
                }
                else if (pack.data.strip.mode == MODE_LIGHTBEAM) {
                    stage = 25;
                }
                else if (pack.data.strip.mode == MODE_RAINBOW) {
                    stage = 26;
                }
                else {
                    return false;
                }
                break;
            case 5:  // write reply cmd
                pack.data.reply.resp = buf[i++];
                stage = 99;
                break;
            case 6:  // ack reply cmd
                pack.data.reply.resp = buf[i++];
                stage = 99;
                break;
            case 7:  // match reply cmd
                uint8_t a, b, c, d;
                a = buf[i++];
                b = buf[i++];
                c = buf[i++];
                d = buf[i++];
                pack.data.ip.addr = PP_HTONL(LWIP_MAKEU32(a,b,c,d));
                stage = 99;
                break;
            case 23:  // write normal mode setting
                pack.data.strip.setting.normal.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // write breathing mode setting
                pack.data.strip.setting.breathing.color = PKG_CONCAT(buf[i++], buf[i++]);
                pack.data.strip.setting.breathing.duration = PKG_CONCAT(buf[i++], buf[i++]);
                pack.data.strip.setting.breathing.interval = PKG_CONCAT(buf[i++], buf[i++]);
                pack.data.strip.setting.breathing.ease = buf[i++];
                stage = 99;
                break;
            case 25:  // write lightbeam mode setting
                pack.data.strip.setting.lightbeam.color = PKG_CONCAT(buf[i++], buf[i++]);
                pack.data.strip.setting.lightbeam.len = buf[i++];
                pack.data.strip.setting.lightbeam.gap = buf[i++];
                pack.data.strip.setting.lightbeam.fade = buf[i++];
                pack.data.strip.setting.lightbeam.head = buf[i++];
                pack.data.strip.setting.lightbeam.tail = buf[i++];
                pack.data.strip.setting.lightbeam.dir = buf[i++];
                pack.data.strip.setting.lightbeam.speed = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 26:  // write rainbow mode setting
                pack.data.strip.setting.rainbow.speed = PKG_CONCAT(buf[i++], buf[i++]);
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

void Package::pack(uint8_t* buf, uint16_t size, const package_t& pack) {
    uint8_t i = 0;

    buf[i++] = PKG_FRAME_HEAD1;
    buf[i++] = PKG_FRAME_HEAD2;
    buf[i++] = 0;  // fill it later
    buf[i++] = pack.cmd;
    if (pack.cmd == CMD_WRITE) {
        buf[i++] = pack.data.strip.power;
        buf[i++] = pack.data.strip.mode;
        if (pack.data.strip.mode == MODE_NORMAL) {
            uint16_t color = pack.data.strip.setting.normal.color;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
        }
        else if (pack.data.strip.mode == MODE_BREATHING) {
            uint16_t color = pack.data.strip.setting.normal.color;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = PKG_HIGH(pack.data.strip.setting.breathing.duration);
            buf[i++] = PKG_LOW(pack.data.strip.setting.breathing.duration);
            buf[i++] = PKG_HIGH(pack.data.strip.setting.breathing.interval);
            buf[i++] = PKG_LOW(pack.data.strip.setting.breathing.interval);
            buf[i++] = pack.data.strip.setting.breathing.ease;
        }
        else if (pack.data.strip.mode == MODE_LIGHTBEAM) {
            uint16_t color = pack.data.strip.setting.normal.color;
            buf[i++] = PKG_HIGH(color);
            buf[i++] = PKG_LOW(color);
            buf[i++] = pack.data.strip.setting.lightbeam.len;
            buf[i++] = pack.data.strip.setting.lightbeam.gap;
            buf[i++] = pack.data.strip.setting.lightbeam.fade;
            buf[i++] = pack.data.strip.setting.lightbeam.head;
            buf[i++] = pack.data.strip.setting.lightbeam.tail;
            buf[i++] = pack.data.strip.setting.lightbeam.dir;
            buf[i++] = PKG_HIGH(pack.data.strip.setting.lightbeam.speed);
            buf[i++] = PKG_LOW(pack.data.strip.setting.lightbeam.speed);
        }
        else if (pack.data.strip.mode == MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(pack.data.strip.setting.rainbow.speed);
            buf[i++] = PKG_LOW(pack.data.strip.setting.rainbow.speed);
        }
    }
    else if (pack.cmd == CMD_READ_REPLY) {
        buf[i++] = pack.data.reply.resp;
    }
    else if (pack.cmd == CMD_MATCH) {
        buf[i++] = ip4_addr1_val(pack.data.ip);
        buf[i++] = ip4_addr2_val(pack.data.ip);
        buf[i++] = ip4_addr3_val(pack.data.ip);
        buf[i++] = ip4_addr4_val(pack.data.ip);
    }
    buf[i++] = PKG_FRAME_TAIL1;
    buf[i++] = PKG_FRAME_TAIL2;

    buf[PKG_BUF_SIZE_INDEX] = i - 4;  // package size
}

void Package::apply(const package_t& pack) {
    if (pack.cmd == CMD_READ_REPLY) {
        config_set_power(pack.data.strip.power);
        config_set_mode(translate_mode(pack.data.strip.mode));

        normal_data_t nd;
        nd.color = lv_color_hex(pack.data.strip.setting.normal.color);
        config_set_normal_data(&nd);

        breathing_data_t bd;
        bd.color = lv_color_hex(pack.data.strip.setting.breathing.color);
        bd.duration = pack.data.strip.setting.breathing.duration;
        bd.interval = pack.data.strip.setting.breathing.interval;
        bd.ease = translate_ease(pack.data.strip.setting.breathing.ease);
        config_set_breathing_data(&bd);

        lightbeam_data_t ld;
        ld.color = lv_color_hex(pack.data.strip.setting.lightbeam.color);
        ld.len = pack.data.strip.setting.lightbeam.len;
        ld.gap = pack.data.strip.setting.lightbeam.gap;
        ld.fade = val_to_fade(pack.data.strip.setting.lightbeam.fade);
        ld.head = pack.data.strip.setting.lightbeam.head;
        ld.tail = pack.data.strip.setting.lightbeam.tail;
        ld.dir = translate_dir(pack.data.strip.setting.lightbeam.dir);
        ld.speed = pack.data.strip.setting.lightbeam.speed;
        config_set_lightbeam_data(&ld);

        rainbow_data_t rb;
        rb.speed = pack.data.strip.setting.rainbow.speed;
        config_set_rainbow_data(&rb);
    }
    else if (pack.cmd == CMD_MATCH_REPLY) {
        
    }
}

void Package::accquire(package_t& pack, cmd_t cmd) {
    pack.cmd = cmd;
    if (cmd == CMD_WRITE) {

    }
    else if (cmd == CMD_MATCH) {
        IPAddress ip = WiFi.localIP();
        pack.data.ip.addr = (uint32_t)ip;
    }
    else if (cmd == CMD_READ_REPLY) {
        pack.data.reply.resp = PKG_OK;
    }
}
