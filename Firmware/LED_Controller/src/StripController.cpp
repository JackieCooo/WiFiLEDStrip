#include "StripController.h"

NeoPixelBusType strip(LED_NUM, LED_PIN);

StripController::StripController() {
    _mode = MODE_NORMAL;
    _displayed = false;

    strip.Begin();
}

void StripController::begin(void) {
    _displayed = true;
}

void StripController::pause(void) {
    _displayed = false;
}

void StripController::routine(void) {
    if (!_displayed) return;

    if (_mode == MODE_NORMAL) {

    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::process();
    }
}

void StripController::clear(void) {
    RgbColor black(0, 0, 0);
    for (uint16_t i = 0; i < LED_NUM; i++) {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

void StripController::setMode(strip_mode_t mode) {
    _mode = mode;
    
}

strip_mode_t StripController::getMode(void) {
    return _mode;
}
