#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global.h"
#include "BaseFeature.h"

struct NormalData {
    RgbColor color;

    NormalData() {}
    NormalData(RgbColor color) : color(color) {}
};

class NormalFeature : public BaseFeature {
public:
    NormalFeature();

    void refresh(void) override;
    void process(void) override;
    void setData(const NormalData& data);
    NormalData getData(void) const;

private:
    bool _refreshed;
    NormalData _data;
};
