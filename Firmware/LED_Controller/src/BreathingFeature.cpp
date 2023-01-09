#include "BreathingFeature.h"

BreathingFeature::BreathingFeature() {
    _cb = bind(&BreathingFeature::_animUpdateFunc, this, placeholders::_1);
    _stage = 0;
}

void BreathingFeature::_animUpdateFunc(const AnimationParam& param) {
    RgbColor updatedColor;
    float progress = _ease(param.progress);
    
    if (param.index == 0) {
        updatedColor = RgbColor::LinearBlend(RgbColor(0), _color, progress);
    }
    else if (param.index == 2) {
        updatedColor = RgbColor::LinearBlend(_color, RgbColor(0), progress);
    }

    if (param.index == 0 || param.index == 2) {
        for (uint16_t pixel = 0; pixel < strip.PixelCount(); pixel++) {
            strip.SetPixelColor(pixel, updatedColor);
        }
        strip.Show();
    }
}

void BreathingFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        switch (_stage)
        {
            case 0:
                _animations.StartAnimation(0, _duration, _cb);
                break;
            case 1:
                _animations.StartAnimation(1, _interval, _cb);
                break;
            case 2:
                _animations.StartAnimation(2, _duration, _cb);
                break;
            case 3:
                _animations.StartAnimation(3, _interval, _cb);
                break;
            default:
                break;
        }
        _stage = (_stage + 1) % STAGE_NUM;
    }
}

void BreathingFeature::reset(void) {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _stage = 0;
}

void BreathingFeature::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);
}

RgbColor BreathingFeature::getColor(void) {
    return _color;
}

void BreathingFeature::setDuration(uint16_t duration) {
    _duration = duration;
}

uint16_t BreathingFeature::getDuartion(void) {
    return _duration;
}

void BreathingFeature::setInterval(uint16_t interval) {
    _interval = interval;
}

uint16_t BreathingFeature::getInterval(void) {
    return _interval;
}

void BreathingFeature::setEase(AnimEaseFunction ease) {
    _ease = ease;
}

AnimEaseFunction BreathingFeature::getEase(void) {
    return _ease;
}

void BreathingFeature::setData(breathing_data_t& data) {
    _color = data.color;
    _duration = data.duration;
    _interval = data.interval;
    _ease = data.ease;
}

breathing_data_t BreathingFeature::getData(void) {
    breathing_data_t data;

    data.color = _color;
    data.duration = _duration;
    data.interval = _interval;
    data.ease = _ease;

    return data;
}
