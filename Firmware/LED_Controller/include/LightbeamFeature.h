#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global_conf.h"

using namespace std;

#define FADED_DISABLE (0)
#define FADED_HEAD (1 << 0)
#define FADED_TAIL (1 << 1)

typedef uint8_t faded_end_t;

typedef enum {
    MOVE_RIGHT,
    MOVE_LEFT
} dir_t;

class LightbeamFeature {
public:
    LightbeamFeature();
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setLen(uint16_t len);
    virtual void setInterval(uint16_t interval);
    void setDirection(dir_t dir);
    virtual void setSpeed(uint16_t speed);
    void setFadedEnd(faded_end_t faded_end);
    void setTailLen(uint16_t len);
    void setHeadLen(uint16_t len);

private:
    RgbColor _color;
    uint16_t _len;
    uint16_t _interval;
    uint16_t _head_len;
    uint16_t _tail_len;
    faded_end_t _faded_end;
    dir_t _dir;
    uint16_t _speed;  // pixels per second
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;

    void _refreshPattern(void);
    void _animUpdateFunc(const AnimationParam& param);
    static uint16_t _translateSpeed(uint16_t &speed);
};
