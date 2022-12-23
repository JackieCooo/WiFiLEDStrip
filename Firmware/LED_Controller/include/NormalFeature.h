#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global_conf.h"

class NormalFeature {
public:
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    virtual RgbColor& getColor(void);

private:
    RgbColor _color;

};
