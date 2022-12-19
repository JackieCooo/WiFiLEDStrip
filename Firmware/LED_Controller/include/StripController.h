#pragma once

#include <NeoPixelBus.h>
#include <BreathingFeature.h>

#include "global_conf.h"

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_INTERVAL,
    MODE_RAINBOW
} strip_mode_t;

class StripController : public BreathingFeature {
public:
    StripController();
    void begin(void);
    void routine(void);
    void pause(void);
    void clear(void);
    void setMode(strip_mode_t mode);
    strip_mode_t getMode(void);

private:
    strip_mode_t _mode;
    bool _displayed;
};
