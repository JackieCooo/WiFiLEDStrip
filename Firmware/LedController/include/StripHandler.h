#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global.h"
#include "BaseFeature.h"
#include "BreathingFeature.h"
#include "NormalFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"
#include "RhythmFeature.h"

using namespace std;

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW,
    MODE_RHYTHM,
    MODE_NUM
} led_mode_t;

class StripHandler {
public:
    StripHandler();

    static void begin();
    void process();
    void refresh();
    static void clear();
    void setMode(led_mode_t mode);
    led_mode_t getMode() const;
    void setPower(bool pwr);
    bool getPower() const;
    void setData(const NormalData& data);
    void setData(const BreathingData& data);
    void setData(const LightbeamData& data);
    void setData(const RainbowData& data);
    void setData(const RhythmData& data);
    void getData(NormalData& data);
    void getData(BreathingData& data);
    void getData(LightbeamData& data);
    void getData(RainbowData& data);
    void getData(RhythmData& data);

    [[noreturn]] static void task(void* args);

private:
    led_mode_t _mode;
    bool _pwr;
    NormalFeature _normal;
    BreathingFeature _breathing;
    LightbeamFeature _lightbeam;
    RainbowFeature _rainbow;
    RhythmFeature _rhythm;
};

extern StripHandler stripHandler;
