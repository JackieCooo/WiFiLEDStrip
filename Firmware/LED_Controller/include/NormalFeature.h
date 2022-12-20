#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global_conf.h"

class NormalFeature {
public:
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    RgbColor _color;

};
