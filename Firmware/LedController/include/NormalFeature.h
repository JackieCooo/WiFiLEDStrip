#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "global.h"
#include "BaseFeature.h"

struct NormalData {
    RgbColor color;

    NormalData() {color = RgbColor(0);}
    explicit NormalData(RgbColor color) : color(color) {}
};

class NormalFeature : public BaseFeature {
public:
    NormalFeature();

    void refresh() override;
    void process() override;
    void setData(const NormalData& data);
    NormalData getData() const;

private:
    bool _refreshed;
    NormalData _data;
};
