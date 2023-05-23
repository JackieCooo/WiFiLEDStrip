#include "StripHandler.h"

NeoPixelBusType strip(LED_NUM, LED_PIN);

StripHandler::StripHandler() {
    begin();
}

void StripHandler::begin(void) {
    strip.Begin();
    clear();
}

void StripHandler::process(void) {
    if (_pwr) {
        switch (_mode)
        {
            case MODE_NORMAL:
                _normal.process();
                break;
            case MODE_BREATHING:
                _breathing.process();
                break;
            case MODE_LIGHTBEAM:
                _lightbeam.process();
                break;
            case MODE_RAINBOW:
                _rainbow.process();
                break;
            default:
                break;
        }
    }
}

void StripHandler::refresh(void) {
    switch (_mode)
    {
        case MODE_NORMAL:
            _normal.refresh();
            break;
        case MODE_BREATHING:
            _breathing.refresh();
            break;
        case MODE_LIGHTBEAM:
            _lightbeam.refresh();
            break;
        case MODE_RAINBOW:
            _rainbow.refresh();
            break;
        default:
            break;
    }
}

void StripHandler::clear(void) {
    RgbColor black(0);
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, black);
    }
    strip.Show();
}

void StripHandler::setMode(led_mode_t mode) {
    refresh();
    this->_mode = mode;
}

led_mode_t StripHandler::getMode(void) const {
    return this->_mode;
}

void StripHandler::setPower(bool pwr) {
    this->_pwr = pwr;
    if (!this->_pwr) {
        clear();
    }
}

bool StripHandler::getPower(void) const {
    return this->_pwr;
}

void StripHandler::setData(const NormalData& data) {
    _normal.setData(data);
}

void StripHandler::setData(const BreathingData& data) {
    _breathing.setData(data);
}

void StripHandler::setData(const LightbeamData& data) {
    _lightbeam.setData(data);
}

void StripHandler::setData(const RainbowData& data) {
    _rainbow.setData(data);
}

void StripHandler::task(void* args) {
    for (;;) {
        stripHandler.process();
        vTaskDelay(10);
    }
}

StripHandler stripHandler;
