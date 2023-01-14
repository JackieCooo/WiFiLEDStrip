#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global.h"

class NormalFeature {
public:
    NormalFeature();
    void process(void);
    void refresh(void);
    void reset(void);

private:

};
