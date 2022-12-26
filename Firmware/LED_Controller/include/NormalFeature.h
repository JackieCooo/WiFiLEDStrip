#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global_conf.h"

typedef struct {
    RgbColor color;
} normal_data_t;

class NormalFeature {
public:
    NormalFeature();
    void process(void);
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b);
    virtual RgbColor getColor(void);
    void setData(normal_data_t& data);
    normal_data_t getData(void);

private:
    RgbColor _color;

};
