#ifndef LVGL_STYLED_SLIDER_H
#define LVGL_STYLED_SLIDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "global.h"

lv_obj_t* create_styled_slider(lv_obj_t* parent);
void styled_slider_set_value(lv_obj_t* obj, int32_t v);
void styled_slider_set_range(lv_obj_t* obj, int32_t min, int32_t max);
int32_t styled_slider_get_value(lv_obj_t* obj);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_STYLED_SLIDER_H
