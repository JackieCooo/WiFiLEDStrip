#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "global.h"

#define STAGE_NUM 4

typedef struct {
    RgbColor color;
    uint16_t duration;
    uint16_t interval;
    AnimEaseFunction ease;
} breathing_data_t;

using namespace std;

class BreathingFeature {
public:
    BreathingFeature();
    void process(void);
    void reset(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setDuration(uint16_t duration);
    void setInterval(uint16_t interval);
    void setEase(AnimEaseFunction ease);
    virtual RgbColor getColor(void);
    uint16_t getDuartion(void);
    uint16_t getInterval(void);
    AnimEaseFunction getEase(void);
    void setData(breathing_data_t& data);
    breathing_data_t getData(void);

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
