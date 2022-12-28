#ifndef LVGL_MAIN_GUI_H
#define LVGL_MAIN_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "lvgl.h"
#include "message_handler.h"
#include "Package.h"

/* User defines */
#define COLOR_CHANGE_MSG 1

/* Enums define */

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW
} led_mode_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* pwr_btn;
    lv_obj_t* color_btn;
    lv_obj_t* mode_btn;
    lv_obj_t* about_btn;
} main_gui_fragment_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* color_selector;
} color_select_fragment_t;

typedef struct {
    lv_fragment_t base;
    lv_obj_t* normal_mode;
    lv_obj_t* breathing_mode;
    lv_obj_t* interval_mode;
    lv_obj_t* rainbow_mode;
} mode_select_fragment_t;

typedef struct {
    lv_fragment_t base;
} about_page_fragment_t;

void create_main_gui(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_MAIN_GUI_H
