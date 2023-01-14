#include "BreathingFeature.h"

BreathingFeature::BreathingFeature() {
    _cb = bind(&BreathingFeature::_animUpdateFunc, this, placeholders::_1);
    _stage = 0;
}

void BreathingFeature::_animUpdateFunc(const AnimationParam& param) {
    RgbColor updatedColor;
    float progress = configuration.setting.breathing.ease(param.progress);
    
    if (param.index == 0) {
        updatedColor = RgbColor::LinearBlend(RgbColor(0), configuration.setting.breathing.color, progress);
    }
    else if (param.index == 2) {
        updatedColor = RgbColor::LinearBlend(configuration.setting.breathing.color, RgbColor(0), progress);
    }

    if (param.index == 0 || param.index == 2) {
        for (uint16_t pixel = 0; pixel < strip.PixelCount(); pixel++) {
            strip.SetPixelColor(pixel, updatedColor);
        }
        strip.Show();
    }
}

void BreathingFeature::process(void) {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        switch (_stage)
        {
            case 0:
                _animations.StartAnimation(0, configuration.setting.breathing.duration, _cb);
                break;
            case 1:
                _animations.StartAnimation(1, configuration.setting.breathing.interval, _cb);
                break;
            case 2:
                _animations.StartAnimation(2, configuration.setting.breathing.duration, _cb);
                break;
            case 3:
                _animations.StartAnimation(3, configuration.setting.breathing.interval, _cb);
                break;
            default:
                break;
        }
        _stage = (_stage + 1) % STAGE_NUM;
    }
}

void BreathingFeature::reset(void) {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _stage = 0;
}

void BreathingFeature::refresh(void) {
    reset();
}
