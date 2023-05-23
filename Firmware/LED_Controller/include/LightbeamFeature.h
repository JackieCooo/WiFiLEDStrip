#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global.h"
#include "BaseFeature.h"

using namespace std;

typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT
} dir_t;

typedef struct {
    uint8_t FADED_HEAD : 1;
    uint8_t FADED_TAIL : 1;
} faded_end_t;

struct LightbeamData {
    RgbColor color;
    uint8_t len;
    uint8_t gap;
    faded_end_t faded_end;
    uint8_t head_len;
    uint8_t tail_len;
    dir_t dir;
    uint16_t speed;

    LightbeamData() {}
    LightbeamData(RgbColor color, uint8_t len, uint8_t gap, faded_end_t faded_end, uint8_t head_len, uint8_t tail_len, dir_t dir, uint16_t speed) : color(color), len(len), gap(gap), faded_end(faded_end), head_len(head_len), tail_len(tail_len), dir(dir), speed(speed) {}
};

class LightbeamFeature : public BaseFeature {
public:
    LightbeamFeature();
    void process(void) override;
    void refresh(void) override;
    void setData(const LightbeamData& data);
    LightbeamData getData(void) const;

private:
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;
    LightbeamData _data;

    void _refreshPattern(void);
    void _animUpdateFunc(const AnimationParam& param);
    static uint16_t _translateSpeed(uint16_t &speed);
};
