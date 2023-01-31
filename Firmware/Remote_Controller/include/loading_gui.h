#ifndef LVGL_LOADING_GUI_H
#define LVGL_LOADING_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void show_loading_gui(const char* text);
void loading_gui_set_text(const char* text);
void hide_loading_gui(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_LOADING_GUI_H
