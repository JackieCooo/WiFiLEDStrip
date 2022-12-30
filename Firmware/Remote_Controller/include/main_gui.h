#ifndef LVGL_MAIN_GUI_H
#define LVGL_MAIN_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "lvgl.h"
#include "btn_group.h"
#include "global.h"
#include "message_handler.h"

typedef struct {
    lv_fragment_t base;
    uint8_t power;
} main_gui_fragment_t;

typedef struct {
    lv_fragment_t base;
} color_select_fragment_t;

typedef struct {
    lv_fragment_t base;
} mode_select_fragment_t;

typedef struct {
    lv_fragment_t base;
} about_page_fragment_t;

void create_main_gui(void);
void refresh_main_gui(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_MAIN_GUI_H
