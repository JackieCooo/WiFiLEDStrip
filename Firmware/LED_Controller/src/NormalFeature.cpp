#include <NormalFeature.h>

NormalFeature::NormalFeature() {
    
}

void NormalFeature::process(void) {
    for (uint16_t i = 0; i < strip.PixelCount(); i++) {
        strip.SetPixelColor(i, configuration.setting.normal.color);
    }
    strip.Show();
}

void NormalFeature::reset(void) {
    return;
}

void NormalFeature::refresh(void) {
    return;
}
