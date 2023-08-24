#include "RhythmFeature.h"

void RhythmFeature::process() {
    for (uint8_t i = 0; i < SAMPLES; ++i) {
        vReal[i] = analogRead(DATA_PIN);
        vImag[i] = 0;
        delayMicroseconds(DELAY_US);
    }

    fft.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    fft.Compute(FFT_FORWARD);
    fft.ComplexToMagnitude();

    RgbColor color = translate(vReal[rhythmData.freq]);
    for (uint16_t i = 0; i < strip.PixelCount(); ++i) {
        strip.SetPixelColor(i, color);
    }
    strip.Show();
}

RhythmFeature::RhythmFeature() {
    pinMode(DATA_PIN, INPUT);
}

RgbColor RhythmFeature::translate(double value) const {
    CLAM(value, MIN_THRESHOLD, MAX_THRESHOLD);
    value -= MIN_THRESHOLD;
    auto strength = (float)(value / (MAX_THRESHOLD - MIN_THRESHOLD));
    return RgbColor::LinearBlend(RgbColor(0), rhythmData.color, strength);
}

const RhythmData &RhythmFeature::getData() const {
    return this->rhythmData;
}

void RhythmFeature::setData(const RhythmData &data) {
    this->rhythmData = data;
}
