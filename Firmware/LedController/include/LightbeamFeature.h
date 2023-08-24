#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <vector>

#include "global.h"
#include "BaseFeature.h"

using namespace std;

typedef enum {
    DIR_BACKWARD,
    DIR_FORWARD,
} dir_t;

struct Fade {
    uint8_t FADED_HEAD : 1;
    uint8_t FADED_TAIL : 1;

    Fade() {FADED_HEAD = 0; FADED_TAIL = 0;}
    Fade(bool head, bool tail) {
        if (head) FADED_HEAD = 1;
        if (tail) FADED_TAIL = 1;
    }
    explicit Fade(uint8_t val) {
        if (val & 0x01) FADED_HEAD = 1;
        if (val & 0x02) FADED_TAIL = 1;
    }
    explicit operator uint8_t() const {
        uint8_t r = 0;
        if (FADED_HEAD) r |= 0x01;
        if (FADED_TAIL) r |= 0x02;
        return r;
    }
};

struct LightbeamData {
    RgbColor color;
    uint8_t len;
    uint8_t gap;
    Fade fade;
    uint8_t head;
    uint8_t tail;
    dir_t dir;
    uint16_t speed;

    LightbeamData() {color = RgbColor(0); len = 0; gap = 0; head = 0; tail = 0; dir = DIR_FORWARD; speed = 0;}
    LightbeamData(RgbColor color, uint8_t len, uint8_t gap, Fade fade, uint8_t head, uint8_t tail, dir_t dir, uint16_t speed) : color(color), len(len), gap(gap), fade(fade), head(head), tail(tail), dir(dir), speed(speed) {}
};

class LightbeamFeature : public BaseFeature {
public:
    LightbeamFeature();
    void process() override;
    void refresh() override;
    void setData(const LightbeamData& data);
    LightbeamData getData() const;

private:
    vector<RgbColor> _pattern;
    uint16_t _ref;  // the first pixel's referance to the pattern
    NeoPixelAnimator _animations = NeoPixelAnimator(1);
    AnimUpdateCallback _cb;
    LightbeamData _data;

    void _refreshPattern();
    void _animUpdateFunc(const AnimationParam& param);
    static uint16_t _translateSpeed(uint16_t &speed);

    void _dump_data() {
#if DEBUG_LOG
        Serial.printf("Color: 0x%04X, Len: %d, Gap: %d, Fade: %02X, Head: %d, Tail: %d, Dir: %02X, Speed: %d\n", Rgb16Color(_data.color).Color565, _data.len, _data.gap, (uint8_t)_data.fade, _data.head, _data.tail, _data.dir, _data.speed);
#endif
    }
};
