#ifndef LVGL_BTN_GROUP_H
#define LVGL_BTN_GROUP_H

#include "lvgl.h"
#include "list.h"

typedef struct {
    list_t* group;
    uint8_t cur_sel;
    uint8_t pre_sel;
    lv_color_t def_color;
    lv_color_t sel_color;
} btngroup_t;

lv_obj_t* btngroup_create(void);
void btngroup_add_btn(lv_obj_t* obj, lv_obj_t* btn);
void btngroup_set_checked(lv_obj_t* obj, uint8_t index);
uint8_t btngroup_get_index(lv_obj_t* obj, lv_obj_t* btn);

extern uint32_t EVENT_SELECTED_CHANGED;

#endif //LVGL_BTN_GROUP_H
