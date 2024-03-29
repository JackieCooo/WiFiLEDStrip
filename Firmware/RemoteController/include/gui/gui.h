#ifndef LVGL_GUI_H
#define LVGL_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <malloc.h>

#include "lvgl.h"
#include "gui/checked_group.h"
#include "gui/animation_handler.h"
#include "gui/styled_spinbox.h"
#include "gui/confirm_panel.h"
#include "gui/wifi_list_item.h"
#include "gui/loading_gui.h"
#include "gui/optional_dialog.h"
#include "message_handler.h"
#include "global.h"

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
    wifi_list_t* wifi_list;
} wifi_scan_fragment_t;

typedef struct {
    lv_fragment_t base;
    char* ssid;
} wifi_connect_fragment_t;

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

void init_gui(void);
void refresh_gui(void);
void clear_gui(void);
void show_connect_gui(void);
void show_main_gui(void);
void show_match_failed_dialog(void);
void show_ack_failed_dialog(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_GUI_H
