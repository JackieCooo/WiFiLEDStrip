#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <BreathingFeature.h>
#include <NormalFeature.h>
#include <LightbeamFeature.h>
#include <RainbowFeature.h>

#include "global_conf.h"

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW,
    MODE_NUM
} strip_mode_t;

class StripHelper : public BreathingFeature, public NormalFeature, public LightbeamFeature, public RainbowFeature {
public:
    void begin(void);
    void routine(void);
    void pause(void);
    void clear(void);
    void setMode(strip_mode_t mode);
    strip_mode_t getMode(void);

    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
    void setInterval(uint16_t interval) override;
    void setSpeed(uint16_t speed) override;

private:
    strip_mode_t _mode;
    bool _displayed;
};
