#include "loading_gui.h"

static lv_obj_t* loading = NULL;

void show_loading_gui(const char* text) {
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(obj, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_bg_opa(obj, 220, 0);
    lv_obj_move_foreground(obj);

    lv_obj_t* spinner = lv_spinner_create(obj, 1000, 60);
    lv_obj_set_size(spinner, lv_pct(50), lv_pct(50));
    lv_obj_align(spinner, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t* label = lv_label_create(obj);
    lv_label_set_text(label, text);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -55);

    loading = obj;
}

void loading_gui_set_text(const char* text) {
    if (loading) {
        lv_obj_t* label = lv_obj_get_child(loading, 1);
        lv_label_set_text(label, text);
    }
}

void hide_loading_gui(void) {
    if (loading) {
        lv_obj_del(loading);
        loading = NULL;
    }
}
