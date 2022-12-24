#include <Package.h>

bool Package::parse(uint8_t* buf, uint8_t size) {
    uint8_t stage = 0;
    uint8_t i = 0;
    for (; i < size;) {
        switch (stage)
        {
            case 0:  // frame head 1
                if (buf[i++] == 0xAB) {
                    stage = 1;
                }
                else return false;
                break;
            case 1:  // frame head 2
                if (buf[i++] == 0xCD) {
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
                    stage = 5;
                }
                else if (_package.cmd == PKG_CMD_ACK) {
                    stage = 6;
                }
                else {
                    return false;
                }
                break;
            case 4:  // write setting cmd
                _package.data.mode = buf[i++];
                if (_package.data.mode == PKG_MODE_NORMAL) {
                    stage = 23;
                }
                else if (_package.data.mode == PKG_MODE_BREATHING) {
                    stage = 24;
                }
                else if (_package.data.mode == PKG_MODE_LIGHTBEAM) {
                    stage = 25;
                }
                else if (_package.data.mode == PKG_MODE_RAINBOW) {
                    stage = 26;
                }
                else {
                    return false;
                }
                break;
            case 23:  // write normal mode setting
                _package.data.settings.normal_setting.color = PKG_CONCAT(buf[i++], buf[i++]);
                stage = 99;
                break;
            case 24:  // write breathing mode setting
                _package.data.settings.breathing_setting.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.breathing_setting.duration = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.breathing_setting.interval = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.breathing_setting.ease = buf[i++];
                stage = 99;
                break;
            case 25:  // write lightbeam mode setting
                _package.data.settings.lightbeam_setting.color = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.interval = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.head_len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.tail_len = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.speed = PKG_CONCAT(buf[i++], buf[i++]);
                _package.data.settings.lightbeam_setting.faded_end = buf[i++];
                _package.data.settings.lightbeam_setting.dir = buf[i++];
                stage = 99;
                break;
            case 26:  // write rainbow mode setting
                _package.data.settings.rainbow_setting.speed = buf[i++];
                stage = 99;
                break;
            case 99:  // frame tail 1
                if (buf[i++] == 0xEE) stage = 100;
                else return false;
                break;
            case 100:  // frmae tail 2
                if (buf[i++] == 0xFF) return true;
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

    buf[i++] = 0xAB;
    buf[i++] = 0xCD;
    buf[i++] = _calPackSize();
    if (_package.cmd == PKG_CMD_READ_SETTING) {
        buf[i++] = PKG_CMD_READ_SETTING;
        buf[i++] = _package.data.mode;
        if (_package.data.mode == PKG_MODE_NORMAL) {
            buf[i++] = PKG_HIGH(_package.data.settings.normal_setting.color);
            buf[i++] = PKG_LOW(_package.data.settings.normal_setting.color);
        }
        else if (_package.data.mode == PKG_MODE_BREATHING) {
            buf[i++] = PKG_HIGH(_package.data.settings.breathing_setting.color);
            buf[i++] = PKG_LOW(_package.data.settings.breathing_setting.color);
            buf[i++] = PKG_HIGH(_package.data.settings.breathing_setting.duration);
            buf[i++] = PKG_LOW(_package.data.settings.breathing_setting.duration);
            buf[i++] = PKG_HIGH(_package.data.settings.breathing_setting.interval);
            buf[i++] = PKG_LOW(_package.data.settings.breathing_setting.interval);
            buf[i++] = _package.data.settings.breathing_setting.ease;
        }
        else if (_package.data.mode == PKG_MODE_LIGHTBEAM) {
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.color);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.color);
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.len);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.len);
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.interval);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.interval);
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.head_len);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.head_len);
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.tail_len);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.tail_len);
            buf[i++] = PKG_HIGH(_package.data.settings.lightbeam_setting.speed);
            buf[i++] = PKG_LOW(_package.data.settings.lightbeam_setting.speed);
            buf[i++] = _package.data.settings.lightbeam_setting.faded_end;
            buf[i++] = _package.data.settings.lightbeam_setting.dir;
        }
        else if (_package.data.mode == PKG_MODE_RAINBOW) {
            buf[i++] = PKG_HIGH(_package.data.settings.rainbow_setting.speed);
            buf[i++] = PKG_LOW(_package.data.settings.rainbow_setting.speed);
        }
    }
    buf[i++] = 0xEE;
    buf[i++] = 0xFF;
}

package_t& Package::getPackage(void) {
    return _package;
}

uint8_t Package::_calPackSize(void) {
    uint8_t size = 2;
    if (_package.cmd == PKG_CMD_READ_SETTING) {
        size += 1;
        if (_package.data.mode == PKG_MODE_NORMAL) {
            size += 2;
        }
        else if (_package.data.mode == PKG_MODE_BREATHING) {
            size += 7;
        }
        else if (_package.data.mode == PKG_MODE_LIGHTBEAM) {
            size += 14;
        }
        else if (_package.data.mode == PKG_MODE_RAINBOW) {
            size += 2;
        }
    }
    else if (_package.cmd == PKG_CMD_ACK) {

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

AnimEaseFunction Package::translateEase(uint8_t& ease) {
    switch (ease)
    {
        case PKG_EASE_LINEAR:
            return NeoEase::Linear;
            break;
        case PKG_EASE_QUADRATIC_IN:
            return NeoEase::QuarticIn;
            break;
        case PKG_EASE_QUADRATIC_OUT:
            return NeoEase::QuarticOut;
            break;
        case PKG_EASE_QUADRATIC_IN_OUT:
            return NeoEase::QuarticInOut;
            break;
        case PKG_EASE_QUADRATIC_CENTER:
            return NeoEase::QuarticCenter;
            break;
        case PKG_EASE_CUBIC_IN:
            return NeoEase::CubicIn;
            break;
        case PKG_EASE_CUBIC_OUT:
            return NeoEase::CubicOut;
            break;
        case PKG_EASE_CUBIC_IN_OUT:
            return NeoEase::CubicInOut;
            break;
        case PKG_EASE_CUBIC_CENTER:
            return NeoEase::CubicCenter;
            break;
        case PKG_EASE_QUARTIC_IN:
            return NeoEase::QuarticIn;
            break;
        case PKG_EASE_QUARTIC_OUT:
            return NeoEase::QuarticOut;
            break;
        case PKG_EASE_QUARTIC_IN_OUT:
            return NeoEase::QuarticInOut;
            break;
        case PKG_EASE_QUARTIC_CENTER:
            return NeoEase::QuarticCenter;
            break;
        case PKG_EASE_QUINTIC_IN:
            return NeoEase::QuinticIn;
            break;
        case PKG_EASE_QUINTIC_OUT:
            return NeoEase::QuinticOut;
            break;
        case PKG_EASE_QUINTIC_IN_OUT:
            return NeoEase::QuinticInOut;
            break;
        case PKG_EASE_QUINTIC_CENTER:
            return NeoEase::QuinticCenter;
            break;
        case PKG_EASE_SINUSOIDAL_IN:
            return NeoEase::SinusoidalIn;
            break;
        case PKG_EASE_SINUSOIDAL_OUT:
            return NeoEase::SinusoidalOut;
            break;
        case PKG_EASE_SINUSOIDAL_IN_OUT:
            return NeoEase::SinusoidalInOut;
            break;
        case PKG_EASE_SINUSOIDAL_CENTER:
            return NeoEase::SinusoidalCenter;
            break;
        case PKG_EASE_EXPONENTIAL_IN:
            return NeoEase::ExponentialIn;
            break;
        case PKG_EASE_EXPONENTIAL_OUT:
            return NeoEase::ExponentialOut;
            break;
        case PKG_EASE_EXPONENTIAL_IN_OUT:
            return NeoEase::ExponentialInOut;
            break;
        case PKG_EASE_EXPONENTIAL_CENTER:
            return NeoEase::ExponentialCenter;
            break;
        case PKG_EASE_CIRCULAR_IN:
            return NeoEase::CircularIn;
            break;
        case PKG_EASE_CIRCULAR_OUT:
            return NeoEase::CircularOut;
            break;
        case PKG_EASE_CIRCULAR_IN_OUT:
            return NeoEase::CircularInOut;
            break;
        case PKG_EASE_CIRCULAR_CENTER:
            return NeoEase::CircularCenter;
            break;
        case PKG_EASE_GAMMA:
            return NeoEase::Gamma;
            break;
        default:
            break;
    }
    return NeoEase::Linear;
}

dir_t Package::translateDirection(uint8_t& dir) {
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
