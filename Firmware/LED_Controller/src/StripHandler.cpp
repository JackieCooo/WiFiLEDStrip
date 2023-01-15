#include <StripHandler.h>

NeoPixelBusType strip(LED_NUM, LED_PIN);

void StripHandler::begin(void) {
    strip.Begin();
    clear();
}

void StripHandler::process(void) {
    if (!configuration.power) return;

    strip_mode_t _mode = configuration.mode;
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

void StripHandler::reset(void) {
    strip_mode_t _mode = configuration.mode;
    if (_mode == MODE_NORMAL) {
        NormalFeature::reset();
    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::reset();
    }
    else if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::reset();
    }
    else if (_mode == MODE_RAINBOW) {
        RainbowFeature::reset();
    }
}

void StripHandler::refresh(void) {
    clear();
    strip_mode_t _mode = configuration.mode;
    if (_mode == MODE_NORMAL) {
        NormalFeature::refresh();
    }
    else if (_mode == MODE_BREATHING) {
        BreathingFeature::refresh();
    }
    else if (_mode == MODE_LIGHTBEAM) {
        LightbeamFeature::refresh();
    }
    else if (_mode == MODE_RAINBOW) {
        RainbowFeature::refresh();
    }
}

void StripHandler::clear(void) {
    RgbColor black(0);
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

StripHandler stripHandler;
