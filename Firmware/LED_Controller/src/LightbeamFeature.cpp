#include <LightbeamFeature.h>

LightbeamFeature::LightbeamFeature() {
    _cb = bind(&LightbeamFeature::_animUpdateFunc, this, placeholders::_1);
    _refreshPattern();
}

void LightbeamFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        _animations.StartAnimation(0, _translateSpeed(configuration.setting.lightbeam.speed), _cb);
    }
}

void LightbeamFeature::reset(void) {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _ref = 0;
}

void LightbeamFeature::refresh(void) {
    reset();
    _refreshPattern();
}

void LightbeamFeature::_refreshPattern(void) {
    _ref = 0;
    RgbColor _color = configuration.setting.lightbeam.color;
    dir_t _dir = configuration.setting.lightbeam.dir;
    uint16_t _len = configuration.setting.lightbeam.len;
    uint16_t _gap = configuration.setting.lightbeam.gap;
    faded_end_t _faded_end = configuration.setting.lightbeam.faded_end;
    uint16_t _head_len = configuration.setting.lightbeam.head_len;
    uint16_t _tail_len = configuration.setting.lightbeam.tail_len;
    uint16_t psize = _len + _gap + (_faded_end.FADED_HEAD ? _tail_len : 0) + (_faded_end.FADED_TAIL ? _head_len : 0);
    _pattern.resize(psize);
    
    uint16_t i = 0;
    NeoGamma<NeoGammaTableMethod> colorGamma;
    if ((_faded_end.FADED_HEAD) && _dir == MOVE_LEFT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_head_len + 1);
        hsl.L = 0;
        for (uint16_t j = 0; j < _head_len; j++) {
            hsl.L += offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    else if ((_faded_end.FADED_TAIL) && _dir == MOVE_RIGHT) {
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
    if ((_faded_end.FADED_HEAD) && _dir == MOVE_RIGHT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_head_len + 1);
        for (uint16_t j = 0; j < _head_len; j++) {
            hsl.L -= offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    else if ((_faded_end.FADED_TAIL) && _dir == MOVE_LEFT) {
        HslColor hsl(_color);
        float offset = hsl.L / (_tail_len + 1);
        for (uint16_t j = 0; j < _tail_len; j++) {
            hsl.L -= offset;
            _pattern[i++] = colorGamma.Correct(RgbColor(hsl));
        }
    }
    for (uint16_t j = 0; j < _gap; ++j) {
        _pattern[i++] = RgbColor(0);
    }
}

void LightbeamFeature::_animUpdateFunc(const AnimationParam& param) {
    if (param.state == AnimationState_Started) {
        for (uint16_t i = 0; i < strip.PixelCount(); ++i) {
            strip.SetPixelColor(i, _pattern[(i+_ref)%_pattern.size()]);
        }
        strip.Show();
        dir_t _dir = configuration.setting.lightbeam.dir;
        if (_dir == MOVE_LEFT) {
            _ref = (++_ref) % _pattern.size();
        }
        else if (_dir == MOVE_RIGHT) {
            _ref = _ref ? --_ref : _pattern.size() - 1;
        }
    }
}

uint16_t LightbeamFeature::_translateSpeed(uint16_t &speed) {
    if (speed > 1000 || speed == 0) return 0;
    return (uint16_t)(1000.0 / (float)speed);
}
