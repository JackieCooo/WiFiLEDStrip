#include <LightbeamFeature.h>

LightbeamFeature::LightbeamFeature() {
}

void LightbeamFeature::process(void) {
    _animations.UpdateAnimations();
}

void LightbeamFeature::_refreshPattern(void) {
    uint16_t psize = _len + _interval;
    _pattern.resize(psize);
    uint16_t i = 0;
    for (uint16_t j = 0; j < _len; ++j) {
        _pattern[i++] = _color;
    }
    for (uint16_t j = 0; j < _interval; ++j) {
        _pattern[i++] = RgbColor(0);
    }
}

void LightbeamFeature::_refreshPixels(void) {
    
}
