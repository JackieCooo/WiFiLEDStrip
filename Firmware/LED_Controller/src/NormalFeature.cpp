#include <NormalFeature.h>

void NormalFeature::process(void) {
    return;
}

void NormalFeature::refresh(void) {
    if (configuration.power) {
        for (uint16_t i = 0; i < strip.PixelCount(); i++) {
            strip.SetPixelColor(i, configuration.setting.normal.color);
        }
        strip.Show();
    }
}
