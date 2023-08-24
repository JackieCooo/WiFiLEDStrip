#include "gui/animation_handler.h"


static void anim_set_x(void* var, int32_t v);
static void anim_set_opa(void* var, int32_t v);


static void anim_set_x(void* var, int32_t v) {
    lv_obj_set_style_translate_x((lv_obj_t*)var, (lv_coord_t)v, 0);
}

static void anim_set_opa(void* var, int32_t v) {
    lv_obj_set_style_bg_opa((lv_obj_t*)var, (lv_opa_t)v, 0);
    lv_obj_set_style_bg_img_opa((lv_obj_t*)var, (lv_opa_t)v, 0);
}

void play_mode_changed_animation(lv_obj_t* pre_item, lv_obj_t* cur_item) {
    // previous item return
    lv_anim_t anim1;
    lv_anim_init(&anim1);
    lv_anim_set_var(&anim1, pre_item);
    lv_anim_set_values(&anim1, 0, 30);
    lv_anim_set_exec_cb(&anim1, anim_set_x);
    lv_anim_set_time(&anim1, 500);
    lv_anim_set_path_cb(&anim1, lv_anim_path_ease_in_out);
    lv_anim_set_early_apply(&anim1, false);

    // previous setting btn hidden
    lv_anim_t anim2;
    lv_anim_init(&anim2);
    lv_anim_set_var(&anim2, lv_obj_get_child(pre_item, 1));
    lv_anim_set_values(&anim2, 255, 0);
    lv_anim_set_exec_cb(&anim2, anim_set_opa);
    lv_anim_set_time(&anim2, 500);
    lv_anim_set_path_cb(&anim2, lv_anim_path_ease_in_out);
    lv_anim_set_early_apply(&anim2, false);

    // current item move
    lv_anim_t anim3;
    lv_anim_init(&anim3);
    lv_anim_set_var(&anim3, cur_item);
    lv_anim_set_values(&anim3, 30, 0);
    lv_anim_set_exec_cb(&anim3, anim_set_x);
    lv_anim_set_time(&anim3, 500);
    lv_anim_set_path_cb(&anim3, lv_anim_path_ease_in_out);
    lv_anim_set_early_apply(&anim3, false);

    // current setting btn reveal
    lv_anim_t anim4;
    lv_anim_init(&anim4);
    lv_anim_set_var(&anim4, lv_obj_get_child(cur_item, 1));
    lv_anim_set_values(&anim4, 0, 255);
    lv_anim_set_exec_cb(&anim4, anim_set_opa);
    lv_anim_set_time(&anim4, 500);
    lv_anim_set_path_cb(&anim4, lv_anim_path_ease_in_out);
    lv_anim_set_early_apply(&anim4, false);

    lv_anim_start(&anim1);
    lv_anim_start(&anim2);
    lv_anim_start(&anim3);
    lv_anim_start(&anim4);
}
