#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "global.h"

#define STAGE_NUM 4

using namespace std;

class BreathingFeature {
public:
    BreathingFeature();
    void process(void);
    void reset(void);
    void refresh(void);

private:
    NeoPixelAnimator _animations = NeoPixelAnimator(STAGE_NUM);
    AnimUpdateCallback _cb;
    uint8_t _stage;

    void _animUpdateFunc(const AnimationParam& param);
    static AnimEaseFunction _translateEase(ease_t& ease);
};
