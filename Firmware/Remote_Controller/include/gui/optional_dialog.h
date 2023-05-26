#ifndef LVGL_OPTIONAL_DIALOG_H
#define LVGL_OPTIONAL_DIALOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "global.h"

#define DIALOG_BTN_W        75
#define DIALOG_BTN_H        30
#define DIALOG_BTN_PANEL_H  (DIALOG_BTN_H + 10)

lv_obj_t* create_optional_dialog(const char* text, const char* left_btn_text, const char* right_btn_text);
void optional_dialog_register_event_id(void);

extern uint32_t EVENT_DIALOG_LEFT_BTN_CLICKED;
extern uint32_t EVENT_DIALOG_RIGHT_BTN_CLICKED;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_OPTIONAL_DIALOG_H
