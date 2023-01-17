#ifndef LVGL_GUI_H
#define LVGL_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "lvgl.h"
#include "checked_group.h"
#include "animation_handler.h"
#include "global.h"
#include "styled_spinbox.h"
#include "confirm_panel.h"

/* User defines */
#define BTN_HOR_RES 120
#define BTN_VER_RES 50
#define MODE_SELECT_ITEM_HOR_RES 180
#define MODE_SELECT_ITEM_VER_RES 50
#define SETTING_BTN_HOR_RES 50
#define SETTING_BTN_VER_RES 50

typedef struct {
    lv_fragment_t base;
} home_fragment_t;

typedef struct {
    lv_fragment_t base;
} color_select_fragment_t;

typedef struct {
    lv_fragment_t base;
} mode_select_fragment_t;

typedef struct {
    lv_fragment_t base;
} about_fragment_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* duration_selector;
    lv_obj_t* interval_selector;
} breathing_setting_fragment_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* len_selector;
    lv_obj_t* gap_selector;
    lv_obj_t* speed_selector;
    lv_obj_t* direction_selector;
    lv_obj_t* faded_end_selector;
    lv_obj_t* head_len_selector;
    lv_obj_t* tail_len_selector;
} lightbeam_setting_fragment_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* speed_selector;
} rainbow_setting_fragment_t;

void create_gui(void);
void refresh_gui(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_GUI_H
