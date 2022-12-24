#include <StripHandler.h>

NeoPixelBusType strip(LED_NUM, LED_PIN);

void StripHandler::begin(void) {
    _mode = MODE_NORMAL;
    _displayed = true;

    strip.Begin();
}

void StripHandler::pause(void) {
    _displayed = false;
}

void StripHandler::routine(void) {
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
    else if (_mode == MODE_RAINBOW) {
        RainbowFeature::process();
    }
}

void StripHandler::clear(void) {
    RgbColor black(0, 0, 0);
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

void StripHandler::setMode(strip_mode_t mode) {
    if (mode == _mode) return;
    _mode = mode;
}

strip_mode_t StripHandler::getMode(void) {
    return _mode;
}

void StripHandler::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (_mode == MODE_NORMAL) {
        NormalFeature::setColor(r, g, b);
    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::setColor(r, g, b);
    }
    else if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::setColor(r, g, b);
    }
}

RgbColor StripHandler::getColor(void) {
    if (_mode == MODE_NORMAL) {
        return NormalFeature::getColor();
    }
    else if (_mode == MODE_BREATHING) {
        return BreathingFeature::getColor();
    }
    else if (_mode == MODE_LIGHTBEAM) {
        return LightbeamFeature::getColor();
    }
    return RgbColor(0);
}

void StripHandler::setInterval(uint16_t interval) {
    if (_mode == MODE_BREATHING) {
        BreathingFeature::setInterval(interval);
    }
    else if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::setInterval(interval);
    }
}

uint16_t StripHandler::getInterval(void) {
    if (_mode == MODE_BREATHING) {
        return BreathingFeature::getInterval();
    }
    else if (_mode == MODE_LIGHTBEAM) {
        return LightbeamFeature::getInterval();
    }
    return 0;
}

void StripHandler::setSpeed(uint16_t speed) {
    if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::setSpeed(speed);
    }
    else if (_mode == MODE_RAINBOW) {
        RainbowFeature::setSpeed(speed);
    }
}

uint16_t StripHandler::getSpeed(void) {
    if (_mode == MODE_LIGHTBEAM) {
        return LightbeamFeature::getSpeed();
    }
    else if (_mode == MODE_RAINBOW) {
        return RainbowFeature::getSpeed();
    }
    return 0;
}

StripHandler stripHandler;
