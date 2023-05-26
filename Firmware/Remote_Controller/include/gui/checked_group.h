#ifndef LVGL_CHECKED_GROUP_H
#define LVGL_CHECKED_GROUP_H

#include "lvgl.h"
#include "list.h"
#include "animation_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    list_t* group;
    uint8_t cur_sel;
    uint8_t pre_sel;
    lv_color_t def_color;
    lv_color_t sel_color;
} checkedgroup_t;

lv_obj_t* checkedgroup_create(void);
void checkedgroup_add_item(lv_obj_t* obj, lv_obj_t* item);
void checkedgroup_set_checked(lv_obj_t* obj, uint8_t index);
uint8_t checkedgroup_get_index(lv_obj_t* obj, lv_obj_t* item);
lv_obj_t* checkedgroup_get_item(lv_obj_t* obj, uint8_t index);
void checkedgroup_set_right_btn_event_cb(lv_obj_t* obj, uint8_t index, lv_event_cb_t cb);

extern uint32_t EVENT_SELECTED_CHANGED;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_CHECKED_GROUP_H
