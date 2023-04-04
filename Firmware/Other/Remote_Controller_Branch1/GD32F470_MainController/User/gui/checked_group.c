#include "checked_group.h"

uint32_t EVENT_SELECTED_CHANGED;

static void left_btn_event_cb(lv_event_t* e);

lv_obj_t* checkedgroup_create(void) {
    lv_obj_t* obj = lv_obj_create(NULL);
    checkedgroup_t* tar = (checkedgroup_t*) lv_mem_alloc(sizeof(checkedgroup_t));
    lv_obj_set_user_data(obj, tar);

    tar->group = list_create();
    tar->cur_sel = 0;
    tar->pre_sel = 0;
    tar->def_color = lv_palette_main(LV_PALETTE_BLUE);
    tar->sel_color = lv_palette_main(LV_PALETTE_RED);

    return obj;
}

void checkedgroup_add_item(lv_obj_t* obj, lv_obj_t* item) {
    checkedgroup_t* tar = (checkedgroup_t*) lv_obj_get_user_data(obj);
    lv_obj_set_style_translate_x(item, 30, 0);

    lv_obj_t* left_btn = lv_obj_get_child(item, 0);
    lv_obj_add_event_cb(left_btn, left_btn_event_cb, LV_EVENT_CLICKED, obj);

    lv_obj_t* right_btn = lv_obj_get_child(item, 1);
    lv_obj_set_style_bg_color(right_btn, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(right_btn, 0, 0);
    lv_obj_set_style_bg_img_opa(right_btn, 0, 0);

    list_push_back(tar->group, item);
}

void checkedgroup_set_checked(lv_obj_t* obj, uint8_t index) {
    checkedgroup_t* tar = (checkedgroup_t*) lv_obj_get_user_data(obj);
    if (tar->group->size <= index) return;

    tar->pre_sel = tar->cur_sel;
    tar->cur_sel = index;
    lv_obj_t* pre_obj = (lv_obj_t*)list_get(tar->group, tar->pre_sel);
    lv_obj_t* cur_obj = (lv_obj_t*)list_get(tar->group, tar->cur_sel);
    lv_obj_set_style_bg_color(lv_obj_get_child(pre_obj, 0), tar->def_color, 0);
    lv_obj_set_style_bg_color(lv_obj_get_child(cur_obj, 0), tar->sel_color, 0);
    play_mode_changed_animation(pre_obj, cur_obj);

    lv_event_send(obj, EVENT_SELECTED_CHANGED, &tar->cur_sel);
}

uint8_t checkedgroup_get_index(lv_obj_t* obj, lv_obj_t* item) {
    checkedgroup_t* tar = (checkedgroup_t*) lv_obj_get_user_data(obj);
    for (uint8_t i = 0; i < tar->group->size; ++i) {
        if (list_get(tar->group, i) == item) {
            return i;
        }
    }
    return tar->group->size;
}

lv_obj_t* checkedgroup_get_item(lv_obj_t* obj, uint8_t index) {
    checkedgroup_t* group = (checkedgroup_t*) lv_obj_get_user_data(obj);
    if (group->group->size <= index) return NULL;
    return (lv_obj_t*) list_get(group->group, index);
}

void checkedgroup_set_right_btn_event_cb(lv_obj_t* obj, uint8_t index, lv_event_cb_t cb) {
    lv_obj_t* item = checkedgroup_get_item(obj, index);
    lv_obj_t* btn = lv_obj_get_child(item, 1);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, obj);
}

static void left_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* item = lv_obj_get_parent(lv_event_get_target(e));
        lv_obj_t* obj = (lv_obj_t*) lv_event_get_user_data(e);
        uint8_t index = checkedgroup_get_index(obj, item);
        checkedgroup_set_checked(obj, index);
    }
}
