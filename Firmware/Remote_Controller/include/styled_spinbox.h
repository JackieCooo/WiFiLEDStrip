#ifndef LVGL_STYLED_SPINBOX_H
#define LVGL_STYLED_SPINBOX_H

#include <stdlib.h>

#include "lvgl.h"
#include "global.h"

#define SPINBOX_H               40
#define SPINBOX_W               100
#define SPINBOX_BTN_H           40
#define SPINBOX_BTN_W           40
#define SPINBOX_PANEL_H         40

#define SPINBOX_TIMEOUT_MS      2000

lv_obj_t* create_styled_spinbox(lv_obj_t* parent);
void styled_spinbox_set_range(lv_obj_t* obj, int32_t min, int32_t max);
void styled_spinbox_set_step(lv_obj_t* obj, uint32_t step);
uint32_t styled_spinbox_get_value(lv_obj_t* obj);

#endif //LVGL_STYLED_SPINBOX_H
