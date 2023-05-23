#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global.h"
#include "BaseFeature.h"

#define COLOR_GAP 16

using namespace std;

struct RainbowData {
    uint16_t speed;

    RainbowData() {}
    RainbowData(uint16_t speed) : speed(speed) {}
};

class RainbowFeature : public BaseFeature {
public:
    RainbowFeature();
    void process(void) override;
    void refresh(void) override;
    void setData(const RainbowData& data);
    RainbowData getData(void) const;

private:
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;
    RainbowData _data;

    void _animUpdateFunc(const AnimationParam& param);
    void _initPattern(void);
    static uint16_t _translateSpeed(uint16_t &speed);
};
