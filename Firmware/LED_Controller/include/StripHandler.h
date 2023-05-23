#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global.h"
#include "BaseFeature.h"
#include "BreathingFeature.h"
#include "NormalFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"

using namespace std;

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW,
    MODE_NUM
} led_mode_t;

class StripHandler {
public:
    StripHandler();

    void begin(void);
    void process(void);
    void refresh(void);
    void clear(void);
    void setMode(led_mode_t mode);
    led_mode_t getMode(void) const;
    void setPower(bool pwr);
    bool getPower(void) const;
    void setData(const NormalData& data);
    void setData(const BreathingData& data);
    void setData(const LightbeamData& data);
    void setData(const RainbowData& data);

    static void task(void* args);

private:
    led_mode_t _mode;
    bool _pwr;
    NormalFeature _normal;
    BreathingFeature _breathing;
    LightbeamFeature _lightbeam;
    RainbowFeature _rainbow;
};

extern StripHandler stripHandler;
