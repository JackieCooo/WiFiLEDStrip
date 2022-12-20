#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global_conf.h"

using namespace std;

#define FADED_HEAD (1 << 0)
#define FADED_TAIL (1 << 1)

typedef uint8_t faded_end_t;

typedef enum {
    MOVE_RIGHT,
    MODE_LEFT
} dir_t;

class LightbeamFeature {
public:
    LightbeamFeature();
    void process(void);

private:
    RgbColor _color;
    uint16_t _len;
    uint16_t _interval;
    uint16_t _head_len;
    uint16_t _tail_len;
    faded_end_t _faded_end;
    dir_t _dir;
    vector<RgbColor> _pattern;
    uint16_t _base;
    NeoPixelAnimator _animations = NeoPixelAnimator(1);

    void _refreshPattern(void);
    void _refreshPixels(void);
};
