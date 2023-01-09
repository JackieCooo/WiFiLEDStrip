#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#define LED_NUM 30
#define LED_PIN 18

typedef NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> NeoPixelBusType;

extern NeoPixelBusType strip;
