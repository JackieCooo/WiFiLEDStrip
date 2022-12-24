#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global_conf.h"

#define COLOR_GAP 16

typedef struct {
    uint16_t speed;
} rainbow_data_t;

using namespace std;

class RainbowFeature {
public:
    RainbowFeature();
    void process(void);
    virtual void setSpeed(uint16_t speed);
    virtual uint16_t getSpeed(void);
    void setData(rainbow_data_t& data);

private:
    uint16_t _speed;
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;

    void _animUpdateFunc(const AnimationParam& param);
    void _initPattern(void);
    static uint16_t _translateSpeed(uint16_t &speed);
};
