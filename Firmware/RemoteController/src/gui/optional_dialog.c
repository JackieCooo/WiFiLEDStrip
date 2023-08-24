#include "gui/optional_dialog.h"

uint32_t EVENT_DIALOG_LEFT_BTN_CLICKED;
uint32_t EVENT_DIALOG_RIGHT_BTN_CLICKED;

static void optional_dialog_left_btn_event_cb(lv_event_t* e);
static void optional_dialog_right_btn_event_cb(lv_event_t* e);

lv_obj_t* create_optional_dialog(const char* text, const char* left_btn_text, const char* right_btn_text) {
    lv_obj_t* bg = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(bg);
    lv_obj_set_size(bg, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(bg, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_bg_opa(bg, 220, 0);
    lv_obj_move_foreground(bg);

    lv_obj_t* dialog = lv_obj_create(bg);
    lv_obj_set_size(dialog, lv_pct(80), lv_pct(60));
    lv_obj_center(dialog);
    lv_obj_set_style_bg_color(dialog, lv_palette_darken(LV_PALETTE_GREY, 2), 0);
    lv_obj_set_style_radius(dialog, 10, 0);
    lv_obj_set_style_border_side(dialog, LV_BORDER_SIDE_NONE, 0);

    lv_obj_t* content = lv_label_create(dialog);
    lv_obj_set_width(content, lv_pct(100));
    lv_label_set_long_mode(content, LV_LABEL_LONG_WRAP);
    lv_label_set_text(content, text);
    lv_obj_set_style_text_align(content, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t* panel = lv_obj_create(dialog);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, lv_pct(100), DIALOG_BTN_PANEL_H);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t* left_btn = lv_btn_create(panel);
    lv_obj_set_size(left_btn, DIALOG_BTN_W, DIALOG_BTN_H);
    lv_obj_set_style_radius(left_btn, lv_pct(50), 0);
    lv_obj_t* left_btn_label = lv_label_create(left_btn);
    lv_obj_set_width(left_btn_label, lv_pct(100));
    lv_label_set_text(left_btn_label, left_btn_text);
    lv_label_set_long_mode(left_btn_label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_align(left_btn_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_event_cb(left_btn, optional_dialog_left_btn_event_cb, LV_EVENT_CLICKED, bg);

    lv_obj_t* right_btn = lv_btn_create(panel);
    lv_obj_set_size(right_btn, DIALOG_BTN_W, DIALOG_BTN_H);
    lv_obj_set_style_radius(right_btn, lv_pct(50), 0);
    lv_obj_t* right_btn_label = lv_label_create(right_btn);
    lv_obj_set_width(right_btn_label, lv_pct(100));
    lv_label_set_text(right_btn_label, right_btn_text);
    lv_label_set_long_mode(right_btn_label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_align(right_btn_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_event_cb(right_btn, optional_dialog_right_btn_event_cb, LV_EVENT_CLICKED, bg);

    return bg;
}

void optional_dialog_register_event_id(void) {
    EVENT_DIALOG_LEFT_BTN_CLICKED = lv_event_register_id();
    EVENT_DIALOG_RIGHT_BTN_CLICKED = lv_event_register_id();
}

static void optional_dialog_left_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_event_send(obj, EVENT_DIALOG_LEFT_BTN_CLICKED, NULL);
    }
}

static void optional_dialog_right_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        lv_event_send(obj, EVENT_DIALOG_RIGHT_BTN_CLICKED, NULL);
    }
}
