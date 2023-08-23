#include <NormalFeature.h>

NormalFeature::NormalFeature() {
    _refreshed = false;
}

void NormalFeature::refresh(void) {
    _refreshed = false;
}

void NormalFeature::process(void) {
    if (!_refreshed) {
        for (uint16_t i = 0; i < strip.PixelCount(); i++) {
            strip.SetPixelColor(i, _data.color);
        }
        strip.Show();
        _refreshed = true;
    }
}

void NormalFeature::setData(const NormalData& data) {
    this->_data = data;
    refresh();
}

NormalData NormalFeature::getData(void) const {
    return this->_data;
}
