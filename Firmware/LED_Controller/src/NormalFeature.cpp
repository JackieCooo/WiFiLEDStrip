#include <NormalFeature.h>

NormalFeature::NormalFeature() {
    _color = RgbColor(0);
}

void NormalFeature::process(void) {
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, _color);
    }
    strip.Show();
}

void NormalFeature::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);
}

RgbColor NormalFeature::getColor(void) {
    return _color;
}

void NormalFeature::setData(normal_data_t& data) {
    _color = data.color;
}

normal_data_t NormalFeature::getData(void) {
    normal_data_t data;

    data.color = _color;

    return data;
}
