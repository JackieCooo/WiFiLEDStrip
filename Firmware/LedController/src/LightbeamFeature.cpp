#include <LightbeamFeature.h>

LightbeamFeature::LightbeamFeature() {
    _ref = 0;
    _cb = bind(&LightbeamFeature::_animUpdateFunc, this, placeholders::_1);
}

void LightbeamFeature::process() {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        _animations.StartAnimation(0, _translateSpeed(_data.speed), _cb);
    }
}

void LightbeamFeature::refresh() {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _ref = 0;
    _refreshPattern();
}

void LightbeamFeature::setData(const LightbeamData& data) {
    this->_data = data;
    _dump_data();
    refresh();
}

LightbeamData LightbeamFeature::getData() const {
    return this->_data;
}

void LightbeamFeature::_refreshPattern() {
    RgbColor _color = _data.color;
    dir_t _dir = _data.dir;
    uint16_t _len = _data.len;
    uint16_t _gap = _data.gap;
    Fade _fade = _data.fade;
    uint16_t _head_len = _data.head;
    uint16_t _tail_len = _data.tail;
    uint16_t psize = _len + _gap + (_fade.FADED_HEAD ? _tail_len : 0) + (_fade.FADED_TAIL ? _head_len : 0);
    _pattern.resize(psize);
    
    uint16_t i = 0;

    if (_fade.FADED_HEAD && _dir == DIR_BACKWARD) {
        uint8_t offset = (uint8_t)(255.0 / (_head_len + 1.0));
        uint8_t val = 0;

        for (uint16_t j = 0; j < _head_len; j++) {
            val += offset;
            _pattern[i++] = _data.color.Dim(val);
        }
    }
    else if (_fade.FADED_TAIL && _dir == DIR_FORWARD) {
        uint8_t offset = (uint8_t)(255.0 / (_tail_len + 1.0));
        uint8_t val = 0;

        for (uint16_t j = 0; j < _tail_len; j++) {
            val += offset;
            _pattern[i++] = _data.color.Dim(val);
        }
    }

    for (uint16_t j = 0; j < _len; ++j) {
        _pattern[i++] = _color;
    }

    if (_fade.FADED_HEAD && _dir == DIR_FORWARD) {
        uint8_t offset = (uint8_t)(255.0 / (_head_len + 1.0));
        uint8_t val = 255;

        for (uint16_t j = 0; j < _head_len; j++) {
            val -= offset;
            _pattern[i++] = _data.color.Dim(val);
        }
    }
    else if (_fade.FADED_TAIL && _dir == DIR_BACKWARD) {
        uint8_t offset = (uint8_t)(255.0 / (_tail_len + 1.0));
        uint8_t val = 255;

        for (uint16_t j = 0; j < _tail_len; j++) {
            val -= offset;
            _pattern[i++] = _data.color.Dim(val);
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

        dir_t _dir = _data.dir;
        if (_dir == DIR_BACKWARD) {
            _ref = (++_ref) % _pattern.size();
        }
        else if (_dir == DIR_FORWARD) {
            _ref = _ref ? --_ref : _pattern.size() - 1;
        }
    }
}

uint16_t LightbeamFeature::_translateSpeed(uint16_t &speed) {
    if (speed > 1000 || speed == 0) return 0;
    return (uint16_t)(1000.0 / (float)speed);
}
