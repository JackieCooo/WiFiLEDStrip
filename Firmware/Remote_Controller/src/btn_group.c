#include "btn_group.h"

uint32_t EVENT_SELECTED_CHANGED;

static void btngroup_value_changed_cb(lv_event_t* e);

lv_obj_t* btngroup_create(void) {
    lv_obj_t* obj = lv_obj_create(NULL);
    btngroup_t* tar = (btngroup_t*) lv_mem_alloc(sizeof(btngroup_t));
    lv_obj_set_user_data(obj, tar);
    EVENT_SELECTED_CHANGED = lv_event_register_id();

    tar->size = 0;
    tar->cur_sel = 0;
    tar->pre_sel = 0;
    tar->def_color = lv_palette_main(LV_PALETTE_BLUE);
    tar->sel_color = lv_palette_main(LV_PALETTE_RED);

    return obj;
}

void btngroup_add_btn(lv_obj_t* obj, lv_obj_t* btn) {
    btngroup_t* tar = (btngroup_t*) lv_obj_get_user_data(obj);
    if (tar->size >= BTNGROUP_MAX_SIZE) return;

    lv_obj_add_event_cb(btn, btngroup_value_changed_cb, LV_EVENT_CLICKED, obj);
    tar->group[tar->size++] = btn;
}

void btngroup_set_checked(lv_obj_t* obj, uint8_t index) {
    btngroup_t* tar = (btngroup_t*) lv_obj_get_user_data(obj);
    if (index >= tar->size) return;

    tar->pre_sel = tar->cur_sel;
    tar->cur_sel = index;
    lv_obj_set_style_bg_color(tar->group[tar->pre_sel], tar->def_color, 0);
    lv_obj_set_style_bg_color(tar->group[tar->cur_sel], tar->sel_color, 0);

    lv_event_send(obj, EVENT_SELECTED_CHANGED, tar);
}

uint8_t btngroup_get_index(lv_obj_t* obj, lv_obj_t* btn) {
    btngroup_t* tar = (btngroup_t*) lv_obj_get_user_data(obj);
    for (uint8_t i = 0; i < tar->size; ++i) {
        if (tar->group[i] == btn) {
            return i;
        }
    }
    return tar->size;
}

static void btngroup_value_changed_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* btn = lv_event_get_target(e);
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        uint8_t index = btngroup_get_index(obj, btn);
        btngroup_set_checked(obj, index);
    }
}
