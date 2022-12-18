#include "strip.h"

NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> _strip(LED_NUM, LED_PIN);

Strip::Strip() {
    _mode = MODE_NORMAL;
    _displayed = false;
    _color = RgbColor(0, 0, 0);
}

void Strip::init(void) {
    _strip.Begin();
}

void Strip::begin(void) {
    _displayed = true;
}

void Strip::pause(void) {
    _displayed = false;
}

void Strip::routine(uint32_t ms) {
    if (!_displayed) return;

    if (_mode == MODE_NORMAL) {
        _onNormalMode();
    }
    else if (_mode == MODE_BREATHING) {
        _onBreathingMode(ms);
    }
    else if (_mode == MODE_INTERVAL) {
        _onIntervalMode(ms);
    }
    // else if (_mode == MODE_RAINBOW) {
    //     _onRainbowMode();
    // }
}

void Strip::clear(void) {
    RgbColor black(0, 0, 0);
    for (uint16_t i = 0; i < LED_NUM; i++) {
        _strip.SetPixelColor(i, black);
    }
    _strip.Show();
}

void Strip::setMode(strip_mode_t mode) {
    _mode = mode;
    
}

strip_mode_t Strip::getMode(void) {
    return _mode;
}

void Strip::setColor(RgbColor color) {
    _color = color;
}

void Strip::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);
}

RgbColor Strip::getColor(void) {
    return _color;
}

void Strip::setBreathingPeriod(uint32_t period) {
    _breathing_data.period = period;
}

void Strip::setInterval(uint32_t interval) {
    _interval_data.interval = interval;
}

void Strip::_resetDataStruct(strip_mode_t &mode) {
    if (mode == MODE_NORMAL) {
        _normal_data.pre = RgbColor(0, 0, 0);
    }
    else if (mode == MODE_BREATHING) {
        _breathing_data.dir = INCREASE;
        _breathing_data.period = 0;
        _breathing_data.stage = 0;
    }
    else if (mode == MODE_INTERVAL) {
        _interval_data.interval = 0;
        _interval_data.stage = 0;
        _interval_data.status = true;
    }
}

void Strip::_onNormalMode(void) {
    if (_color != _normal_data.pre) {
        for (uint16_t i = 0; i < LED_NUM; i++) {
            _strip.SetPixelColor(i, _color);
        }
        _strip.Show();
        _normal_data.pre = _color;
    }
}

void Strip::_onBreathingMode(uint32_t ms) {
    HsbColor hsb(_color);
    float creament = hsb.B / _breathing_data.period;
    hsb.B = _breathing_data.stage * creament;
    for (uint16_t i = 0; i < LED_NUM; i++) {
        _strip.SetPixelColor(i, hsb);
    }
    _strip.Show();
    if (_breathing_data.dir == INCREASE) {
        _breathing_data.stage += ms;
        if (_breathing_data.stage > _breathing_data.period) {
            _breathing_data.stage = _breathing_data.period;
            _breathing_data.dir = DECREASE;
        }
    }
    else {
        _breathing_data.stage -= ms;
        if (_breathing_data.stage < 0) {
            _breathing_data.stage = 0;
            _breathing_data.dir = INCREASE;
        }
    }
}

void Strip::_onIntervalMode(uint32_t ms) {
    if (_interval_data.stage > _interval_data.interval) {
        _interval_data.stage = 0;

    }
}
