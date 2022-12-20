#include <Arduino.h>
#include <StripHelper.h>

StripHelper helper;

NeoPixelAnimator anim(1);

void setup() {
  Serial.begin(115200);

  // helper.begin();
  // helper.setMode(MODE_LIGHTBEAM);

  strip.SetPixelColor(0, RgbColor(0, 0, 255));
  strip.SetPixelColor(1, RgbColor(0, 255, 0));
  strip.SetPixelColor(2, RgbColor(255, 0, 0));

  AnimUpdateCallback cb = [](const AnimationParam& param) {
    if (param.state == AnimationState_Completed) {
      anim.RestartAnimation(param.index);
      strip.RotateRight(1);
    }
  };

  anim.StartAnimation(0, 500, cb);
}

void loop() {
  // helper.routine();
  anim.UpdateAnimations();
  strip.Show();
}
