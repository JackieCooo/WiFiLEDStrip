#include <RainbowFeature.h>

RainbowFeature::RainbowFeature() {
    _cb = bind(&RainbowFeature::_animUpdateFunc, this, placeholders::_1);
    _initPattern();
    _speed = 0;
}

void RainbowFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        _animations.StartAnimation(0, _speed, _cb);
    }
}

void RainbowFeature::setSpeed(uint16_t speed) {
    _speed = _translateSpeed(speed);
}

uint16_t RainbowFeature::getSpeed(void) {
    return _speed;
}

void RainbowFeature::setData(rainbow_data_t& data) {
    _speed = _translateSpeed(data.speed);
}

rainbow_data_t RainbowFeature::getData(void) {
    rainbow_data_t data;

    data.speed = _speed;

    return data;
}

void RainbowFeature::_animUpdateFunc(const AnimationParam& param) {
    if (param.state == AnimationState_Started) {
        for (uint16_t i = 0; i < strip.PixelCount(); ++i) {
            strip.SetPixelColor(i, _pattern[(i+_ref)%_pattern.size()]);
        }
        strip.Show();
        _ref = (++_ref) % _pattern.size();
    }
}

void RainbowFeature::_initPattern(void) {
    uint8_t pixels_per_stage = 256 / COLOR_GAP;
    uint16_t psize = 6 * (uint16_t)pixels_per_stage;
    _pattern.resize(psize);
    uint16_t i = 0;

    for (uint8_t j = 0; j < 6; j++) {
        for (uint8_t k = 0, c = 0; k < pixels_per_stage; k++, c += COLOR_GAP) {
            switch (j)
            {
            case 0:
                _pattern[i++] = RgbColor(255, c, 0);
                break;
            case 1:
                _pattern[i++] = RgbColor(255 - c, 255, 0);
                break;
            case 2:
                _pattern[i++] = RgbColor(0, 255, c);
                break;
            case 3:
                _pattern[i++] = RgbColor(0, 255 - c, 255);
                break;
            case 4:
                _pattern[i++] = RgbColor(c, 0, 255);
                break;
            case 5:
                _pattern[i++] = RgbColor(255, 0, 255 - c);
                break;
            default:
                break;
            }
        }
    }
}

uint16_t RainbowFeature::_translateSpeed(uint16_t &speed) {
    if (speed > 1000) return 0;
    return (uint16_t)(1000.0 / (float)speed);
}
