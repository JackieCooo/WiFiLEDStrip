#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "global_conf.h"

class BreathingFeature {
public:
    BreathingFeature();
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setDuration(uint32_t duration);
    void setInterval(uint32_t interval);
    void setEase(AnimEaseFunction ease);

private:
    const uint8_t _anim_num = 4;
    RgbColor _color;
    uint32_t _duration;
    uint32_t _interval;
    AnimEaseFunction _ease;
    NeoPixelAnimator _animations = NeoPixelAnimator(_anim_num);
    AnimUpdateCallback _cb;
    uint8_t _stage;

    void _animUpdateFunc(const AnimationParam& param);
};
