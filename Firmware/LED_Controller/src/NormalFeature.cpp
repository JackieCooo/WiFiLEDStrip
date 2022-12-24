#include <NormalFeature.h>

NormalFeature::NormalFeature() {
    _color = RgbColor(0);
}

void NormalFeature::process(void) {

}

void NormalFeature::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = RgbColor(r, g, b);

    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, _color);
    }
    strip.Show();
}

RgbColor NormalFeature::getColor(void) {
    return _color;
}

void NormalFeature::setData(normal_data_t& data) {
    _color = data.color;

    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, _color);
    }
    strip.Show();
}
