#pragma once

#include <Arduino.h>
#include <arduinoFFT.h>
#include <NeoPixelBus.h>

#include "global.h"
#include "BaseFeature.h"

#define CLAM(VAL, MIN, MAX) (VAL = VAL < MIN ? MIN : (VAL > MAX ? MAX : VAL))

typedef enum {
    FREQ_0HZ,
    FREQ_300HZ,
    FREQ_600HZ,
    FREQ_900HZ,
    FREQ_1200HZ,
    FREQ_1500HZ,
    FREQ_1800HZ,
    FREQ_2100HZ,
    FREQ_2400HZ,
    FREQ_2700HZ,
    FREQ_3000HZ,
    FREQ_3300HZ,
    FREQ_3600HZ,
    FREQ_3900HZ,
    FREQ_4200HZ,
    FREQ_4500HZ,
} freq_enum;

struct RhythmData {
    RgbColor color;
    freq_enum freq;

    RhythmData() {color = RgbColor(0); freq = FREQ_0HZ;}
    RhythmData(RgbColor color, freq_enum freq) : color(color), freq(freq) {}
};

class RhythmFeature : public BaseFeature {
public:
    RhythmFeature();
    void process() override;
    const RhythmData &getData() const;
    void setData(const RhythmData &data);

private:
    static constexpr uint8_t DATA_PIN = 34;
    static constexpr uint8_t SAMPLES = 32;
    static constexpr uint16_t SAMPLE_FREQ = 9600;
    static constexpr uint16_t DELAY_US = (uint16_t)(1000000.0 / SAMPLE_FREQ);
    static constexpr double MIN_THRESHOLD = 500.0;
    static constexpr double MAX_THRESHOLD = 5000.0;

    double vReal[SAMPLES] = {0}, vImag[SAMPLES] = {0};
    arduinoFFT fft = arduinoFFT(vReal, vImag, SAMPLES, SAMPLE_FREQ);
    RhythmData rhythmData;

    RgbColor translate(double value) const;
};