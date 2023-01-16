#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "BreathingFeature.h"
#include "NormalFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"
#include "global.h"

class StripHandler : public BreathingFeature, public NormalFeature, public LightbeamFeature, public RainbowFeature {
public:
    void begin(void);
    void process(void);
    void refresh(void);
    void reset(void);
    void clear(void);
    static void task(void* args);

private:
};

extern StripHandler stripHandler;
