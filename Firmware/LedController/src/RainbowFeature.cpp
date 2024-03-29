#include <RainbowFeature.h>

RainbowFeature::RainbowFeature() {
    _ref = 0;
    _cb = bind(&RainbowFeature::_animUpdateFunc, this, placeholders::_1);
    _initPattern();
}

void RainbowFeature::process() {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        _animations.StartAnimation(0, _translateSpeed(_data.speed), _cb);
    }
}

void RainbowFeature::refresh() {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _ref = 0;
}

void RainbowFeature::setData(const RainbowData& data) {
    this->_data = data;
    refresh();
}

RainbowData RainbowFeature::getData() const {
    return this->_data;
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

void RainbowFeature::_initPattern() {
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
    if (speed > 1000 || speed == 0) return 0;
    return (uint16_t)(1000.0 / (float)speed);
}
