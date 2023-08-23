#include "BreathingFeature.h"

BreathingFeature::BreathingFeature() {
    _cb = bind(&BreathingFeature::_animUpdateFunc, this, placeholders::_1);
    _stage = 0;
}

void BreathingFeature::process() {
    if (_animations.IsAnimating()) {
        _animations.UpdateAnimations();
    }
    else {
        switch (_stage)
        {
            case 0:
                _animations.StartAnimation(0, _data.duration, _cb);
                break;
            case 1:
                _animations.StartAnimation(1, _data.interval, _cb);
                break;
            case 2:
                _animations.StartAnimation(2, _data.duration, _cb);
                break;
            case 3:
                _animations.StartAnimation(3, _data.interval, _cb);
                break;
            default:
                break;
        }
        _stage = (_stage + 1) % STAGE_NUM;
    }
}

void BreathingFeature::refresh() {
    if (_animations.IsAnimating()) {
        _animations.StopAll();
    }
    _stage = 0;
}

void BreathingFeature::setData(const BreathingData& data) {
    this->_data = data;
    refresh();
}

BreathingData BreathingFeature::getData() const {
    return this->_data;
}

void BreathingFeature::_animUpdateFunc(const AnimationParam& param) {
    RgbColor updatedColor;
    AnimEaseFunction func = _translateEase(_data.ease);
    float progress = func(param.progress);
    
    if (param.index == 0) {
        updatedColor = RgbColor::LinearBlend(RgbColor(0), _data.color, progress);
    }
    else if (param.index == 2) {
        updatedColor = RgbColor::LinearBlend(_data.color, RgbColor(0), progress);
    }

    if (param.index == 0 || param.index == 2) {
        for (uint16_t pixel = 0; pixel < strip.PixelCount(); pixel++) {
            strip.SetPixelColor(pixel, updatedColor);
        }
        strip.Show();
    }
}

AnimEaseFunction BreathingFeature::_translateEase(ease_t& ease) {
    switch (ease)
    {
        case EASE_LINEAR:
            return NeoEase::Linear;
            break;
        case EASE_QUADRATIC_IN:
            return NeoEase::QuadraticIn;
            break;
        case EASE_QUADRATIC_OUT:
            return NeoEase::QuadraticOut;
            break;
        case EASE_QUADRATIC_IN_OUT:
            return NeoEase::QuadraticInOut;
            break;
        case EASE_QUADRATIC_CENTER:
            return NeoEase::QuadraticCenter;
            break;
        case EASE_CUBIC_IN:
            return NeoEase::CubicIn;
            break;
        case EASE_CUBIC_OUT:
            return NeoEase::CubicOut;
            break;
        case EASE_CUBIC_IN_OUT:
            return NeoEase::CubicInOut;
            break;
        case EASE_CUBIC_CENTER:
            return NeoEase::CubicCenter;
            break;
        case EASE_QUARTIC_IN:
            return NeoEase::QuarticIn;
            break;
        case EASE_QUARTIC_OUT:
            return NeoEase::QuarticOut;
            break;
        case EASE_QUARTIC_IN_OUT:
            return NeoEase::QuarticInOut;
            break;
        case EASE_QUARTIC_CENTER:
            return NeoEase::QuarticCenter;
            break;
        case EASE_QUINTIC_IN:
            return NeoEase::QuinticIn;
            break;
        case EASE_QUINTIC_OUT:
            return NeoEase::QuinticOut;
            break;
        case EASE_QUINTIC_IN_OUT:
            return NeoEase::QuinticInOut;
            break;
        case EASE_QUINTIC_CENTER:
            return NeoEase::QuinticCenter;
            break;
        case EASE_SINUSOIDAL_IN:
            return NeoEase::SinusoidalIn;
            break;
        case EASE_SINUSOIDAL_OUT:
            return NeoEase::SinusoidalOut;
            break;
        case EASE_SINUSOIDAL_IN_OUT:
            return NeoEase::SinusoidalInOut;
            break;
        case EASE_SINUSOIDAL_CENTER:
            return NeoEase::SinusoidalCenter;
            break;
        case EASE_EXPONENTIAL_IN:
            return NeoEase::ExponentialIn;
            break;
        case EASE_EXPONENTIAL_OUT:
            return NeoEase::ExponentialOut;
            break;
        case EASE_EXPONENTIAL_IN_OUT:
            return NeoEase::ExponentialInOut;
            break;
        case EASE_EXPONENTIAL_CENTER:
            return NeoEase::ExponentialCenter;
            break;
        case EASE_CIRCULAR_IN:
            return NeoEase::CircularIn;
            break;
        case EASE_CIRCULAR_OUT:
            return NeoEase::CircularOut;
            break;
        case EASE_CIRCULAR_IN_OUT:
            return NeoEase::CircularInOut;
            break;
        case EASE_CIRCULAR_CENTER:
            return NeoEase::CircularCenter;
            break;
        case EASE_GAMMA:
            return NeoEase::Gamma;
            break;
        default:
            break;
    }
    return NeoEase::Linear;
}

