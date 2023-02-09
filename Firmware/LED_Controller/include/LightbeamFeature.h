#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global.h"

using namespace std;

class LightbeamFeature {
public:
    LightbeamFeature();
    void process(void);
    void refresh(void);

private:
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;

    void _refreshPattern(void);
    void _animUpdateFunc(const AnimationParam& param);
    static uint16_t _translateSpeed(uint16_t &speed);
};
