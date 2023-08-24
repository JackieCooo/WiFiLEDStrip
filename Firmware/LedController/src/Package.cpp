#include "Package.h"

bool Package::parse(const uint8_t* buf, uint16_t size, package_t& pack) {
    uint8_t stage = 0;
    uint8_t i = 0;

    for (; i < size;) {
        switch (stage)
        {
            case STAGE_FRAME_HEAD1:  // frame head 1
                if (buf[i++] == PKG_FRAME_HEAD1) {
                    stage = STAGE_FRAME_HEAD2;
                }
                else return false;
                break;
            case STAGE_FRAME_HEAD2:  // frame head 2
                if (buf[i++] == PKG_FRAME_HEAD2) {
                    stage = STAGE_SIZE_BYTE;
                }
                else return false;
                break;
            case STAGE_SIZE_BYTE:  // size byte
                pack.size = buf[i++];
                stage = STAGE_CMD_BYTE;
                break;
            case STAGE_CMD_BYTE:  // cmd byte
                pack.cmd = buf[i++];
                if (PKG_CHECK_CMD(pack, CMD_WRITE)) {
                    stage = STAGE_CMD_WRITE;
                }
                else if (PKG_CHECK_CMD(pack, CMD_READ)) {
                    stage = STAGE_FRAME_TAIL1;
                }
                else if (PKG_CHECK_CMD(pack, CMD_ACK)) {
                    stage = STAGE_FRAME_TAIL1;
                }
                else if (PKG_CHECK_CMD(pack, CMD_MATCH)) {
                    stage = STAGE_CMD_MATCH;
                }
                else {
                    return false;
                }
                break;
            case STAGE_CMD_WRITE:  // write setting cmd
                pack.data.strip.power = buf[i++];
                pack.data.strip.mode = buf[i++];
                if (PKG_CHECK_MODE(pack, MODE_NORMAL)) {
                    stage = STAGE_WRITE_NORMAL;
                }
                else if (PKG_CHECK_MODE(pack, MODE_BREATHING)) {
                    stage = STAGE_WRITE_BREATHING;
                }
                else if (PKG_CHECK_MODE(pack, MODE_LIGHTBEAM)) {
                    stage = STAGE_WRITE_LIGHTBEAM;
                }
                else if (PKG_CHECK_MODE(pack, MODE_RAINBOW)) {
                    stage = STAGE_WRITE_RAINBOW;
                }
                else if (PKG_CHECK_MODE(pack, MODE_RHYTHM)) {
                    stage = STAGE_WRITE_RHYTHM;
                }
                else {
                    return false;
                }
                break;
            case STAGE_CMD_MATCH:  // match cmd
                uint8_t a, b, c, d;
                a = buf[i++];
                b = buf[i++];
                c = buf[i++];
                d = buf[i++];
                pack.data.ip.addr = PP_HTONL(LWIP_MAKEU32(a,b,c,d));
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_WRITE_NORMAL:  // write normal mode setting
                PKG_VAL(pack, normal, color) = PKG_CONCAT(buf[i++], buf[i++]);
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_WRITE_BREATHING:  // write breathing mode setting
                PKG_VAL(pack, breathing, color) = PKG_CONCAT(buf[i++], buf[i++]);
                PKG_VAL(pack, breathing, duration) = PKG_CONCAT(buf[i++], buf[i++]);
                PKG_VAL(pack, breathing, interval) = PKG_CONCAT(buf[i++], buf[i++]);
                PKG_VAL(pack, breathing, ease) = buf[i++];
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_WRITE_LIGHTBEAM:  // write lightbeam mode setting
                PKG_VAL(pack, lightbeam, color) = PKG_CONCAT(buf[i++], buf[i++]);
                PKG_VAL(pack, lightbeam, len) = buf[i++];
                PKG_VAL(pack, lightbeam, gap) = buf[i++];
                PKG_VAL(pack, lightbeam, fade) = buf[i++];
                PKG_VAL(pack, lightbeam, head) = buf[i++];
                PKG_VAL(pack, lightbeam, tail) = buf[i++];
                PKG_VAL(pack, lightbeam, dir) = buf[i++];
                PKG_VAL(pack, lightbeam, speed) = PKG_CONCAT(buf[i++], buf[i++]);
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_WRITE_RAINBOW:  // write rainbow mode setting
                PKG_VAL(pack, rainbow, speed) = PKG_CONCAT(buf[i++], buf[i++]);
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_WRITE_RHYTHM:  // write rhythm mode setting
                PKG_VAL(pack, rhythm, color) = PKG_CONCAT(buf[i++], buf[i++]);
                PKG_VAL(pack, rhythm, freq) = buf[i++];
                stage = STAGE_FRAME_TAIL1;
                break;
            case STAGE_FRAME_TAIL1:  // frame tail 1
                if (buf[i++] == PKG_FRAME_TAIL1) stage = STAGE_FRAME_TAIL2;
                else return false;
                break;
            case STAGE_FRAME_TAIL2:  // frame tail 2
                if (buf[i] == PKG_FRAME_TAIL2) return true;
                else return false;
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
    if (PKG_CHECK_CMD(pack, CMD_READ_REPLY)) {
        buf[i++] = pack.data.strip.power;
        buf[i++] = pack.data.strip.mode;
        if (PKG_CHECK_MODE(pack, MODE_NORMAL)) {
            buf[i++] = PKG_HIGH(PKG_VAL(pack, normal, color));
            buf[i++] = PKG_LOW(PKG_VAL(pack, normal, color));
        }
        else if (PKG_CHECK_MODE(pack, MODE_BREATHING)) {
            buf[i++] = PKG_HIGH(PKG_VAL(pack, breathing, color));
            buf[i++] = PKG_LOW(PKG_VAL(pack, breathing, color));
            buf[i++] = PKG_HIGH(PKG_VAL(pack, breathing, duration));
            buf[i++] = PKG_LOW(PKG_VAL(pack, breathing, duration));
            buf[i++] = PKG_HIGH(PKG_VAL(pack, breathing, interval));
            buf[i++] = PKG_LOW(PKG_VAL(pack, breathing, interval));
            buf[i++] = PKG_VAL(pack, breathing, ease);
        }
        else if (PKG_CHECK_MODE(pack, MODE_LIGHTBEAM)) {
            buf[i++] = PKG_HIGH(PKG_VAL(pack, lightbeam, color));
            buf[i++] = PKG_LOW(PKG_VAL(pack, lightbeam, color));
            buf[i++] = PKG_VAL(pack, lightbeam, len);
            buf[i++] = PKG_VAL(pack, lightbeam, gap);
            buf[i++] = PKG_VAL(pack, lightbeam, fade);
            buf[i++] = PKG_VAL(pack, lightbeam, head);
            buf[i++] = PKG_VAL(pack, lightbeam, tail);
            buf[i++] = PKG_VAL(pack, lightbeam, dir);
            buf[i++] = PKG_HIGH(PKG_VAL(pack, lightbeam, speed));
            buf[i++] = PKG_LOW(PKG_VAL(pack, lightbeam, speed));
        }
        else if (PKG_CHECK_MODE(pack, MODE_RAINBOW)) {
            buf[i++] = PKG_HIGH(PKG_VAL(pack, rainbow, speed));
            buf[i++] = PKG_LOW(PKG_VAL(pack, rainbow, speed));
        }
        else if (PKG_CHECK_MODE(pack, MODE_RHYTHM)) {
            buf[i++] = PKG_HIGH(PKG_VAL(pack, rhythm, color));
            buf[i++] = PKG_LOW(PKG_VAL(pack, rhythm, color));
            buf[i++] = PKG_VAL(pack, rhythm, freq);
        }
    }
    else if (PKG_CHECK_CMD(pack, CMD_WRITE_REPLY)) {
        buf[i++] = pack.data.reply.resp;
    }
    else if (PKG_CHECK_CMD(pack, CMD_ACK_REPLY)) {
        buf[i++] = pack.data.reply.resp;
    }
    else if (PKG_CHECK_CMD(pack, CMD_MATCH_REPLY)) {
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
    if (PKG_CHECK_CMD(pack, CMD_WRITE)) {
        stripHandler.setPower(pack.data.strip.power);
        stripHandler.setMode(static_cast<led_mode_t>(pack.data.strip.mode));
        if (PKG_CHECK_MODE(pack, MODE_NORMAL)) {
            stripHandler.setData(
                    NormalData(
                            RgbColor(Rgb16Color(PKG_VAL(pack, normal, color)))
                    )
            );
        }
        else if (PKG_CHECK_MODE(pack, MODE_BREATHING)) {
            stripHandler.setData(
                    BreathingData(
                            RgbColor(Rgb16Color(PKG_VAL(pack, breathing, color))),
                            PKG_VAL(pack, breathing, duration),
                            PKG_VAL(pack, breathing, interval),
                            static_cast<ease_t>(PKG_VAL(pack, breathing, ease))
                    )
            );
        }
        else if (PKG_CHECK_MODE(pack, MODE_LIGHTBEAM)) {
            stripHandler.setData(
                    LightbeamData(
                            RgbColor(Rgb16Color(PKG_VAL(pack, lightbeam, color))),
                            PKG_VAL(pack, lightbeam, len),
                            PKG_VAL(pack, lightbeam, gap),
                            Fade(PKG_VAL(pack, lightbeam, fade)),
                            PKG_VAL(pack, lightbeam, head),
                            PKG_VAL(pack, lightbeam, tail),
                            static_cast<dir_t>(PKG_VAL(pack, lightbeam, dir)),
                            PKG_VAL(pack, lightbeam, speed)
                    )
            );
        }
        else if (PKG_CHECK_MODE(pack, MODE_RAINBOW)) {
            stripHandler.setData(
                    RainbowData(
                            PKG_VAL(pack, rainbow, speed)
                    )
            );
        }
        else if (PKG_CHECK_MODE(pack, MODE_RHYTHM)) {
            stripHandler.setData(
                    RhythmData(
                            RgbColor(Rgb16Color(PKG_VAL(pack, rhythm, color))),
                            static_cast<freq_enum>(PKG_VAL(pack, rhythm, freq))
                    )
            );
        }
    }
}

void Package::accquire(package_t& pack, cmd_t cmd) {
    pack.cmd = cmd;
    if (cmd == CMD_READ_REPLY) {
        pack.data.strip.power = stripHandler.getPower();

        led_mode_t mode = stripHandler.getMode();
        if (mode == MODE_NORMAL) {
            NormalData data;
            stripHandler.getData(data);
            
            PKG_SET_DATA(pack, normal, color, (Rgb16Color(data.color)).Color565);
        }
        else if (mode == MODE_BREATHING) {
            BreathingData data;
            stripHandler.getData(data);

            PKG_SET_DATA(pack, breathing, color, (Rgb16Color(data.color)).Color565);
            PKG_SET_DATA(pack, breathing, duration, data.duration);
            PKG_SET_DATA(pack, breathing, interval, data.interval);
            PKG_SET_DATA(pack, breathing, ease, data.ease);
        }
        else if (mode == MODE_LIGHTBEAM) {
            LightbeamData data;
            stripHandler.getData(data);
            
            PKG_SET_DATA(pack, lightbeam, color, (Rgb16Color(data.color)).Color565);
            PKG_SET_DATA(pack, lightbeam, len, data.len);
            PKG_SET_DATA(pack, lightbeam, gap, data.gap);
            PKG_SET_DATA(pack, lightbeam, fade, (uint8_t)(data.fade));
            PKG_SET_DATA(pack, lightbeam, head, data.head);
            PKG_SET_DATA(pack, lightbeam, tail, data.tail);
            PKG_SET_DATA(pack, lightbeam, dir, data.dir);
            PKG_SET_DATA(pack, lightbeam, speed, data.speed);
        }
        else if (mode == MODE_RAINBOW) {
            RainbowData data;
            stripHandler.getData(data);
            
            PKG_SET_DATA(pack, rainbow, speed, data.speed);
        }
        else if (mode == MODE_RHYTHM) {
            RhythmData data;
            stripHandler.getData(data);
            
            PKG_SET_DATA(pack, rhythm, color, (Rgb16Color(data.color)).Color565);
            PKG_SET_DATA(pack, rhythm, freq, data.freq);
        }
    }
    else if (cmd == CMD_MATCH_REPLY) {
        IPAddress ip = WiFi.localIP();
        pack.data.ip.addr = (uint32_t)ip;
    }
    else if (cmd == CMD_ACK_REPLY) {
        pack.data.reply.resp = PKG_OK;
    }
    else if (cmd == CMD_WRITE_REPLY) {
        pack.data.reply.resp = PKG_OK;
    }
}
