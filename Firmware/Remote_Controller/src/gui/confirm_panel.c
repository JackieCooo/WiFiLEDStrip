#include "confirm_panel.h"

uint32_t EVENT_CONFIRM_PANEL_CONFIRMED;
uint32_t EVENT_CONFIRM_PANEL_APPLIED;

static void confirm_btn_event_cb(lv_event_t* e);
static void apply_btn_event_cb(lv_event_t* e);

lv_obj_t* create_confirm_panel(lv_obj_t* parent) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(panel, lv_pct(100), CONFIRM_PANEL_H);
    lv_obj_set_style_pad_gap(panel, 15, 0);

    lv_obj_t* confirm_btn = lv_btn_create(panel);
    lv_obj_set_size(confirm_btn, CONFIRM_BTN_W, CONFIRM_BTN_H);
    lv_obj_set_style_radius(confirm_btn, CONFIRM_BTN_H / 2, 0);
    lv_obj_t* confirm_btn_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_btn_label, "Confirm");
    lv_obj_center(confirm_btn_label);
    lv_obj_add_event_cb(confirm_btn, confirm_btn_event_cb, LV_EVENT_CLICKED, panel);

    lv_obj_t* apply_btn = lv_btn_create(panel);
    lv_obj_set_size(apply_btn, CONFIRM_BTN_W, CONFIRM_BTN_H);
    lv_obj_set_style_radius(apply_btn, CONFIRM_BTN_H / 2, 0);
    lv_obj_t* apply_btn_label = lv_label_create(apply_btn);
    lv_label_set_text(apply_btn_label, "Apply");
    lv_obj_center(apply_btn_label);
    lv_obj_add_event_cb(apply_btn, apply_btn_event_cb, LV_EVENT_CLICKED, panel);

    return panel;
}

void confirm_panel_register_event_id(void) {
    EVENT_CONFIRM_PANEL_CONFIRMED = lv_event_register_id();
    EVENT_CONFIRM_PANEL_APPLIED = lv_event_register_id();
}

static void confirm_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_event_send(obj, EVENT_CONFIRM_PANEL_CONFIRMED, NULL);
    }
}

static void apply_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_event_send(obj, EVENT_CONFIRM_PANEL_APPLIED, NULL);
    }
}
