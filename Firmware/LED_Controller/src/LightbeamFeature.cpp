#include <LightbeamFeature.h>

LightbeamFeature::LightbeamFeature() {
    _cb = bind(&LightbeamFeature::_animUpdateFunc, this, placeholders::_1);
    _color = RgbColor(0);
    _len = 0;
    _interval = 0;
    _dir = MOVE_RIGHT;
    _speed = 0;
    _faded_end = FADED_DISABLE;
    _tail_len = 0;
    _head_len = 0;
}

void LightbeamFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        _animations.StartAnimation(0, _speed, _cb);
    }
}

void LightbeamFeature::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);
    _refreshPattern();
}

void LightbeamFeature::setLen(uint16_t len) {
    _len = len;
    _refreshPattern();
}

void LightbeamFeature::setInterval(uint16_t interval) {
    _interval = interval;
    _refreshPattern();
}

void LightbeamFeature::setDirection(dir_t dir) {
    _dir = dir;
}

void LightbeamFeature::setSpeed(uint16_t speed) {
    _speed = _translateSpeed(speed);
}

void LightbeamFeature::setFadedEnd(faded_end_t faded_end) {
    _faded_end = faded_end;
    _refreshPattern();
}

void LightbeamFeature::setTailLen(uint16_t len) {
    _tail_len = len;
    _refreshPattern();
}

void LightbeamFeature::setHeadLen(uint16_t len) {
    _head_len = len;
    _refreshPattern();
}

void LightbeamFeature::_refreshPattern(void) {
    _ref = 0;
    uint16_t psize = _len + _interval + (_faded_end & FADED_TAIL ? _tail_len : 0) + (_faded_end & FADED_HEAD ? _head_len : 0);
    _pattern.resize(psize);
    
    uint16_t i = 0;
    NeoGamma<NeoGammaTableMethod> colorGamma;
    if ((_faded_end & FADED_HEAD) && _dir == MOVE_LEFT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_head_len + 1);
        hsl.L = 0;
        for (uint16_t j = 0; j < _head_len; j++) {
            hsl.L += offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    else if ((_faded_end & FADED_TAIL) && _dir == MOVE_RIGHT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_tail_len + 1);
        hsl.L = 0;
        for (uint16_t j = 0; j < _tail_len; j++) {
            hsl.L += offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    for (uint16_t j = 0; j < _len; ++j) {
        _pattern[i++] = _color;
    }
    if ((_faded_end & FADED_HEAD) && _dir == MOVE_RIGHT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_head_len + 1);
        for (uint16_t j = 0; j < _head_len; j++) {
            hsl.L -= offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    else if ((_faded_end & FADED_TAIL) && _dir == MOVE_LEFT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_tail_len + 1);
        for (uint16_t j = 0; j < _tail_len; j++) {
            hsl.L -= offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    for (uint16_t j = 0; j < _interval; ++j) {
        _pattern[i++] = RgbColor(0);
    }
}

void LightbeamFeature::_animUpdateFunc(const AnimationParam& param) {
    if (param.state == AnimationState_Started) {
        for (uint16_t i = 0; i < strip.PixelCount(); ++i) {
            strip.SetPixelColor(i, _pattern[(i+_ref)%_pattern.size()]);
        }
        strip.Show();
        if (_dir == MOVE_LEFT) {
            _ref = (++_ref) % _pattern.size();
        }
        else if (_dir == MOVE_RIGHT) {
            _ref = _ref ? --_ref : _pattern.size() - 1;
        }
    }
}

uint16_t LightbeamFeature::_translateSpeed(uint16_t &speed) {
    if (speed > 1000) return 0;
    return (uint16_t)(1000.0 / (float)speed);
}
