#ifndef LVGL_CONFIRM_PANEL_H
#define LVGL_CONFIRM_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "global.h"

#define CONFIRM_BTN_W 80
#define CONFIRM_BTN_H 40
#define CONFIRM_PANEL_H 60

void confirm_panel_register_event_id(void);
lv_obj_t* create_confirm_panel(lv_obj_t* parent);

extern uint32_t EVENT_CONFIRM_PANEL_CONFIRMED;
extern uint32_t EVENT_CONFIRM_PANEL_APPLIED;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_CONFIRM_PANEL_H
