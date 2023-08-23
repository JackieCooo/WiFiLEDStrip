#include "RhythmFeature.h"

void RhythmFeature::process() {
    for (uint8_t i = 0; i < SAMPLES; ++i) {
        vReal[i] = analogRead(DATA_PIN);
        vImag[i] = 0;
        delayMicroseconds(DELAY_US);
    }


}

RhythmFeature::RhythmFeature() {
    pinMode(DATA_PIN, INPUT);
}
