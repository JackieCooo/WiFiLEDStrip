#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <lvgl.h>
#include <Arduino_ST7789.h>

#define ST7789_DC    -1
#define ST7789_RST   25
#define ST7789_CS    33   // only for displays with CS pin
#define ST7789_MOSI  26   // for hardware SPI data pin (all of available pins)
#define ST7789_SCLK  27   // for hardware SPI sclk pin (all of available pins)
#define ST7789_BLC   32

class Display
{
public:
    void init(void);
};

#endif