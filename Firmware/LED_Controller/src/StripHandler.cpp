#include <StripHandler.h>

NeoPixelBusType strip(LED_NUM, LED_PIN);

void StripHandler::begin(void) {
    strip.Begin();
    refresh();
}

void StripHandler::process(void) {
    if (!configuration.power) return;

    led_mode_t _mode = configuration.mode;
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

void StripHandler::refresh(void) {
    clear();
    led_mode_t _mode = configuration.mode;
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

void StripHandler::task(void* args) {
    for (;;) {
        stripHandler.process();
        vTaskDelay(10);
    }
}

StripHandler stripHandler;
