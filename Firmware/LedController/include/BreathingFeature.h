#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "global.h"
#include "BaseFeature.h"

#define STAGE_NUM 4

using namespace std;

typedef enum {
    EASE_LINEAR,
    EASE_QUADRATIC_IN,
    EASE_QUADRATIC_OUT,
    EASE_QUADRATIC_IN_OUT,
    EASE_QUADRATIC_CENTER,
    EASE_CUBIC_IN,
    EASE_CUBIC_OUT,
    EASE_CUBIC_IN_OUT,
    EASE_CUBIC_CENTER,
    EASE_QUARTIC_IN,
    EASE_QUARTIC_OUT,
    EASE_QUARTIC_IN_OUT,
    EASE_QUARTIC_CENTER,
    EASE_QUINTIC_IN,
    EASE_QUINTIC_OUT,
    EASE_QUINTIC_IN_OUT,
    EASE_QUINTIC_CENTER,
    EASE_SINUSOIDAL_IN,
    EASE_SINUSOIDAL_OUT,
    EASE_SINUSOIDAL_IN_OUT,
    EASE_SINUSOIDAL_CENTER,
    EASE_EXPONENTIAL_IN,
    EASE_EXPONENTIAL_OUT,
    EASE_EXPONENTIAL_IN_OUT,
    EASE_EXPONENTIAL_CENTER,
    EASE_CIRCULAR_IN,
    EASE_CIRCULAR_OUT,
    EASE_CIRCULAR_IN_OUT,
    EASE_CIRCULAR_CENTER,
    EASE_GAMMA
} ease_t;

struct BreathingData {
    RgbColor color;
    uint16_t duration;
    uint16_t interval;
    ease_t ease;

    BreathingData() {color = RgbColor(0); duration = 0; interval = 0; ease = EASE_LINEAR;}
    BreathingData(RgbColor color, uint16_t duration, uint16_t interval, ease_t ease) : color(color), duration(duration), interval(interval), ease(ease) {}
};

class BreathingFeature : public BaseFeature {
public:
    BreathingFeature();
    void process() override;
    void refresh() override;
    void setData(const BreathingData& data);
    BreathingData getData() const;

private:
    NeoPixelAnimator _animations = NeoPixelAnimator(STAGE_NUM);
    AnimUpdateCallback _cb;
    uint8_t _stage;
    BreathingData _data;

    void _animUpdateFunc(const AnimationParam& param);
    static AnimEaseFunction _translateEase(ease_t& ease);
};
