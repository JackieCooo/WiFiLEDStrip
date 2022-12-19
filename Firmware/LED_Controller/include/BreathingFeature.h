#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "global_conf.h"

class BreathingFeature {
public:
    BreathingFeature();
    void process(void);
    void setColor(const RgbColor &color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setDuration(uint32_t duration);
    void setInterval(uint32_t interval);
    void setEase(AnimEaseFunction ease);

private:
    RgbColor _color;
    uint32_t _duration;
    uint32_t _interval;
    AnimEaseFunction _ease;
    NeoPixelAnimator _animations = NeoPixelAnimator(2);
    AnimUpdateCallback _cb;

    void _animUpdateFunc(const AnimationParam& param);
    inline void _processAnim(void);
};
