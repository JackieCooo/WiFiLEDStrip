#ifndef LVGL_LOADING_GUI_H
#define LVGL_LOADING_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

lv_obj_t* show_loading_gui(const char* text);

inline void hide_loading_gui(lv_obj_t* obj) {
    lv_obj_del(obj);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_LOADING_GUI_H
