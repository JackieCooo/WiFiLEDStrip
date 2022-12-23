#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "global_conf.h"

#define STAGE_NUM 4

using namespace std;

class BreathingFeature {
public:
    BreathingFeature();
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setDuration(uint16_t duration);
    virtual void setInterval(uint16_t interval);
    void setEase(AnimEaseFunction ease);
    virtual RgbColor& getColor(void);
    uint16_t& getDuartion(void);
    virtual uint16_t& getInterval(void);
    AnimEaseFunction getEase(void);

private:
    RgbColor _color;
    uint16_t _duration;
    uint16_t _interval;
    AnimEaseFunction _ease;
    NeoPixelAnimator _animations = NeoPixelAnimator(STAGE_NUM);
    AnimUpdateCallback _cb;
    uint8_t _stage;

    void _animUpdateFunc(const AnimationParam& param);
};