#ifndef _INDEV_H
#define _INDEV_H

#include <lvgl.h>
#include "CST816D.h"

#define CST816D_SDA 21
#define CST816D_SCL 22

class Indev
{
public:
    void init(void);
};

#endif
