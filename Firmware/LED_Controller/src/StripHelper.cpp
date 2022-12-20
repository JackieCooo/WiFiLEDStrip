#include "StripHelper.h"

NeoPixelBusType strip(LED_NUM, LED_PIN);

void StripHelper::begin(void) {
    _mode = MODE_NORMAL;
    _displayed = true;

    strip.Begin();
    strip.Show();
}

void StripHelper::pause(void) {
    _displayed = false;
}

void StripHelper::routine(void) {
    if (!_displayed) return;

    if (_mode == MODE_NORMAL) {
        NormalFeature::process();
    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::process();
    }
    else if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::process();
    }
}

void StripHelper::clear(void) {
    RgbColor black(0, 0, 0);
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

void StripHelper::setMode(strip_mode_t mode) {
    if (mode == _mode) return;
    _mode = mode;
}

strip_mode_t StripHelper::getMode(void) {
    return _mode;
}

void StripHelper::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (_mode == MODE_NORMAL) {
        NormalFeature::setColor(r, g, b);
    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::setColor(r, g, b);
    }
}
