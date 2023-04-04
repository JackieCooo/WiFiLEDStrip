#ifndef LVGL_CONFIRM_PANEL_H
#define LVGL_CONFIRM_PANEL_H

#include "lvgl.h"
#include "global.h"

#define CONFIRM_BTN_W 80
#define CONFIRM_BTN_H 40
#define CONFIRM_PANEL_H 60

lv_obj_t* create_confirm_panel(lv_obj_t* parent);

extern uint32_t EVENT_CONFIRMED;
extern uint32_t EVENT_APPLIED;

#endif //LVGL_CONFIRM_PANEL_H
