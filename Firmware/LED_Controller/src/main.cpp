#include <Arduino.h>
#include <StripController.h>

StripController helper;

void setup() {
  Serial.begin(115200);

  helper.setMode(MODE_BREATHING);
  helper.BreathingFeature::setColor(0, 0, 255);
  helper.BreathingFeature::setDuration(2000);
  helper.BreathingFeature::setInterval(1000);
  helper.BreathingFeature::setEase(NeoEase::Linear);
  helper.begin();
}

void loop() {
  helper.routine();
}