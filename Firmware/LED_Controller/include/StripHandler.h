#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <BreathingFeature.h>
#include <NormalFeature.h>
#include <LightbeamFeature.h>
#include <RainbowFeature.h>

#include "global.h"

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW,
    MODE_NUM
} strip_mode_t;

class StripHandler : public BreathingFeature, public NormalFeature, public LightbeamFeature, public RainbowFeature {
public:
    void begin(void);
    void routine(void);
    void clear(void);
    void setMode(strip_mode_t mode);
    strip_mode_t getMode(void);
    void setPower(bool power);
    bool getPower(void);

    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
    void setSpeed(uint16_t speed) override;
    RgbColor getColor(void) override;
    uint16_t getSpeed(void) override;

private:
    strip_mode_t _mode;
    bool _power;
};

extern StripHandler stripHandler;
