#include "BreathingFeature.h"

BreathingFeature::BreathingFeature() {
    _cb = std::bind(&BreathingFeature::_animUpdateFunc, this, std::placeholders::_1);
}

void BreathingFeature::_animUpdateFunc(const AnimationParam& param) {
    RgbColor updatedColor;
    float progress = _ease(param.progress);
    
    if (param.index == 0) {
        updatedColor = RgbColor::LinearBlend(RgbColor(0), _color, progress);
    }
    else if (param.index == 1) {
        updatedColor = RgbColor::LinearBlend(_color, RgbColor(0), progress);
    }

    for (uint16_t pixel = 0; pixel < strip.PixelCount(); pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
}

void BreathingFeature::process(void) {
    _animations.StartAnimation(0, _duration, _cb);
    _processAnim();
    delay(_interval);
    _animations.StartAnimation(1, _duration, _cb);
    _processAnim();
    delay(_interval);
}

inline void BreathingFeature::_processAnim(void) {
    while(_animations.IsAnimating()) {
        _animations.UpdateAnimations();
        strip.Show();
    }
}

void BreathingFeature::setColor(const RgbColor &color) {
    _color = color;
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
