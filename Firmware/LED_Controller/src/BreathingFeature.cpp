#include "BreathingFeature.h"

BreathingFeature::BreathingFeature() {
    _cb = std::bind(&BreathingFeature::_animUpdateFunc, this, std::placeholders::_1);
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
    }
}

void BreathingFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
        strip.Show();
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
        _stage = (_stage + 1) % _anim_num;
    }
}

void BreathingFeature::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);
}

void BreathingFeature::setDuration(uint32_t duration) {
    _duration = duration;
}

void BreathingFeature::setInterval(uint32_t interval) {
    _interval = interval;
}

void BreathingFeature::setEase(AnimEaseFunction ease) {
    _ease = ease;
}
