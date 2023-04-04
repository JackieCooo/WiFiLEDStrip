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
#define DEFAULT_BTN_W                   150
#define DEFAULT_BTN_H                   60
#define SETTING_BTN_W                   DEFAULT_BTN_H
#define SETTING_BTN_H                   DEFAULT_BTN_H
#define MODE_SELECT_ITEM_W              (DEFAULT_BTN_W + SETTING_BTN_W + 20)
#define MODE_SELECT_ITEM_H              DEFAULT_BTN_H

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
} breathing_setting_fragment_t;

typedef struct {
    lv_fragment_t base;
} lightbeam_setting_fragment_t;

typedef struct {
    lv_fragment_t base;
} rainbow_setting_fragment_t;

void create_gui(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_GUI_H
