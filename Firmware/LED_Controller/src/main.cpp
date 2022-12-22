#include <Arduino.h>
#include <StripHelper.h>

StripHelper helper;

void setup() {
  Serial.begin(115200);

  helper.begin();
  helper.setMode(MODE_RAINBOW);
  helper.setSpeed(8);
}

void loop() {
  helper.routine();
}
