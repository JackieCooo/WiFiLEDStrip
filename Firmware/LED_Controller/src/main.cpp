#include <Arduino.h>

#include "strip.h"

Strip strip;

void setup() {
  Serial.begin(115200);

  strip.init();
  strip.setMode(MODE_BREATHING);
  strip.setBreathingPeriod(1000);
  strip.setColor(255, 0, 0);
  strip.begin();
}

void loop() {
  strip.routine(5);
  delay(5);
}