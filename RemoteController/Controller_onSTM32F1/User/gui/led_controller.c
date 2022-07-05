#include "./gui/led_controller.h"

static void btn_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_t* target = lv_event_get_target(e);
        uint8_t* status = (uint8_t*)lv_event_get_user_data(e);
        lv_obj_t* label = lv_obj_get_child(target, 0);
        printf("clicked\n");
        if (*status)
        {
            lv_obj_set_style_bg_color(target, lv_palette_main(LV_PALETTE_RED), 0);
            lv_label_set_text(label, "Closed");
        }
        else
        {
            lv_obj_set_style_bg_color(target, lv_palette_main(LV_PALETTE_GREEN), 0);
            lv_label_set_text(label, "Opened");
        }
        *status = !(*status);
    }
}

static void colorwheel_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t* tar = lv_event_get_target(e);
        lv_color_t rgb = lv_colorwheel_get_rgb(tar);
        printf("color: [%d, %d, %d]\n", 8*rgb.ch.red, 4*rgb.ch.green, 8*rgb.ch.blue);
    }
}

void interface_init(void)
{
    uint8_t cur_status = 0;

    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, &cur_status);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_size(btn, 100, 50);
//    lv_obj_set_style_outline_width(btn, 0, 0);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, "Closed");
    lv_obj_center(label);

    lv_obj_t* color_selector = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_add_event_cb(color_selector, colorwheel_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_colorwheel_set_mode(color_selector, LV_COLORWHEEL_MODE_HUE);
    lv_colorwheel_set_mode_fixed(color_selector, true);
    lv_obj_set_width(color_selector, 200);
    lv_obj_set_height(color_selector, 200);
    lv_obj_align(color_selector, LV_ALIGN_BOTTOM_MID, 0, -30);
}
