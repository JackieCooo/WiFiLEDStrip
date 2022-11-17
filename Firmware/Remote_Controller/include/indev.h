#ifndef _INDEV_H
#define _INDEV_H

#include <lvgl.h>
#include <focaltech.h>
#include <Wire.h>

#define FT6236_SDA 22
#define FT6236_SCL 21

class Indev
{
public:
    void init(void);
};

#endif
