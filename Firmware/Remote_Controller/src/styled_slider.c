#include "styled_slider.h"

static void slider_event_cb(lv_event_t* e);

lv_obj_t* create_styled_slider(lv_obj_t* parent) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, lv_pct(100), 50);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(panel, 10, 0);
    lv_obj_set_style_pad_top(panel, 10, 0);
    lv_obj_center(panel);

    lv_obj_t* slider = lv_slider_create(panel);
    lv_obj_set_width(slider, lv_pct(85));
    lv_slider_set_value(slider, 0, LV_ANIM_OFF);

    lv_obj_t* label = lv_label_create(panel);
    lv_label_set_text(label, "0");

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, label);

    return panel;
}

void styled_slider_set_value(lv_obj_t* obj, int32_t v) {
    lv_obj_t* slider = lv_obj_get_child(obj, 0);
    lv_slider_set_value(slider, v, LV_ANIM_OFF);
}

void styled_slider_set_range(lv_obj_t* obj, int32_t min, int32_t max) {
    lv_obj_t* slider = lv_obj_get_child(obj, 0);
    lv_slider_set_range(slider, min, max);
}

int32_t styled_slider_get_value(lv_obj_t* obj) {
    lv_obj_t* slider = lv_obj_get_child(obj, 0);
    return lv_slider_get_value(slider);
}

static void slider_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t* slider = lv_event_get_target(e);
        lv_obj_t* label = (lv_obj_t*) lv_event_get_user_data(e);

        char text[8];
        lv_snprintf(text, sizeof(text), "%d", lv_slider_get_value(slider));
        lv_label_set_text(label, text);
    }
}
