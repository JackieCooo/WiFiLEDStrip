#ifndef LVGL_WIFI_LIST_ITEM_H
#define LVGL_WIFI_LIST_ITEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#define WIFI_LIST_ITEM_H        50

lv_obj_t* create_wifi_list_item(lv_obj_t* parent, char* ssid);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_WIFI_LIST_ITEM_H
