#pragma once

#include <NeoPixelBus.h>

#define LED_NUM 10
#define LED_PIN 18

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_INTERVAL,
    MODE_RAINBOW
} strip_mode_t;

typedef enum {
    INCREASE,
    DECREASE
} dir_t;

typedef struct {
    RgbColor pre;  // previous color
} strip_normal_mode_data_t;

typedef struct {
    uint32_t period;  // breathing period
    int32_t stage;  // current stage
    dir_t dir;  // stage changing direction (increase or decrease)
} strip_breathing_mode_data_t;

typedef struct {
    uint32_t interval;
    uint32_t stage;
    bool status;  // on or off
    uint16_t gap;  // 
} strip_interval_mode_data_t;

class Strip {
public:
    Strip();
    void init(void);
    void begin(void);
    void routine(uint32_t ms);
    void pause(void);
    void clear(void);
    void setMode(strip_mode_t mode);
    strip_mode_t getMode(void);
    void setColor(RgbColor color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    RgbColor getColor(void);
    void setBreathingPeriod(uint32_t period);
    void setInterval(uint32_t interval);

private:
    strip_mode_t _mode;
    bool _displayed;
    RgbColor _color;
    strip_normal_mode_data_t _normal_data;
    strip_breathing_mode_data_t _breathing_data;
    strip_interval_mode_data_t _interval_data;

    void _resetDataStruct(strip_mode_t &mode);
    void _onNormalMode(void);
    void _onBreathingMode(uint32_t ms);
    void _onIntervalMode(uint32_t ms);
    // void _onRainbowMode(strip_mode_data_t data);
};
