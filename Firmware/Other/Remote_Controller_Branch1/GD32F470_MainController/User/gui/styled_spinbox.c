#include "styled_spinbox.h"

static void styled_spinbox_decrease_event_cb(lv_event_t* e);
static void styled_spinbox_increase_event_cb(lv_event_t* e);

lv_obj_t* create_styled_spinbox(lv_obj_t* parent) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(panel, 5, 0);
    lv_obj_set_size(panel, lv_pct(100), SPINBOX_PANEL_H);

    lv_obj_t* left_btn = lv_btn_create(panel);
    lv_obj_set_size(left_btn, SPINBOX_BTN_W, SPINBOX_BTN_H);
    lv_obj_set_style_bg_img_src(left_btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(left_btn, styled_spinbox_decrease_event_cb, LV_EVENT_CLICKED, panel);

    lv_obj_t* spinbox = lv_spinbox_create(panel);
    lv_obj_set_size(spinbox, SPINBOX_W, SPINBOX_H);
    lv_obj_set_style_bg_opa(spinbox, 0, LV_PART_CURSOR);
    lv_spinbox_set_rollover(spinbox, false);
    lv_spinbox_set_digit_format(spinbox, 4, 0);
    lv_textarea_set_cursor_click_pos(spinbox, false);

    lv_obj_t* right_btn = lv_btn_create(panel);
    lv_obj_set_size(right_btn, SPINBOX_BTN_W, SPINBOX_BTN_H);
    lv_obj_set_style_bg_img_src(right_btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(right_btn, styled_spinbox_increase_event_cb, LV_EVENT_CLICKED, panel);

    return panel;
}

void styled_spinbox_set_range(lv_obj_t* obj, int32_t min, int32_t max) {
    lv_obj_t* spinbox = lv_obj_get_child(obj, 1);
    lv_spinbox_set_range(spinbox, min, max);
}

void styled_spinbox_set_step(lv_obj_t* obj, uint32_t step) {
    lv_obj_t* spinbox = lv_obj_get_child(obj, 1);
    lv_spinbox_set_step(spinbox, step);
}

uint32_t styled_spinbox_get_value(lv_obj_t* obj) {
    lv_obj_t* spinbox = lv_obj_get_child(obj, 1);
    const char* text = lv_textarea_get_text(spinbox);
    return atoi(text);
}

static void styled_spinbox_decrease_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_obj_t* spinbox = lv_obj_get_child(obj, 1);
        lv_spinbox_decrement(spinbox);
    }
}

static void styled_spinbox_increase_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_obj_t* spinbox = lv_obj_get_child(obj, 1);
        lv_spinbox_increment(spinbox);
    }
}
