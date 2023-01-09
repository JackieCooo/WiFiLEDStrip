#include <StripHandler.h>

NeoPixelBusType strip(LED_NUM, LED_PIN);

void StripHandler::begin(void) {
    _mode = MODE_NORMAL;
    _power = false;

    strip.Begin();
    clear();
}

void StripHandler::routine(void) {
    if (!_power) return;

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
    RgbColor black(0);
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

void StripHandler::setPower(bool power) {
    _power = power;
    if (!_power) {
        if (_mode == MODE_BREATHING) {
            BreathingFeature::reset();
        }
        else if (_mode == MODE_LIGHTBEAM) {
            LightbeamFeature::reset();
        }
        else if (_mode == MODE_RAINBOW) {
            RainbowFeature::reset();
        }
        clear();
    }
}

bool StripHandler::getPower(void) {
    return _power;
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
