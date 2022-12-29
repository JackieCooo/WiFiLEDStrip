#include "main_gui.h"

/* Static variables define */

static lv_obj_t* container;
static lv_fragment_manager_t* manager;
static lv_fragment_t* main_gui_fragment;
static lv_fragment_t* color_select_fragment;
static lv_fragment_t* mode_select_fragment;
static lv_fragment_t* about_page_fragment;

static uint8_t pwr = 0;
static lv_color_t pre_color;
static lv_color_t cur_color;
static led_mode_t pre_mode = MODE_NORMAL;
static led_mode_t cur_mode = MODE_NORMAL;


/* Static functions define */

static lv_obj_t* main_gui_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* about_page_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static void container_del_cb(lv_event_t* e);
static void pwr_btn_event_cb(lv_event_t* e);
static void color_btn_event_cb(lv_event_t* e);
static void mode_btn_event_cb(lv_event_t* e);
static void about_btn_event_cb(lv_event_t* e);
static void color_changed_cb(lv_event_t* e);
static void return_gesture_cb(lv_event_t* e);
static void mode_select_cb(lv_event_t* e);

static void btn_set_text(lv_obj_t* btn, const char* text);


/* Static structs define */

static const lv_fragment_class_t main_gui_cls = {
    .create_obj_cb = main_gui_create_cb,
    .instance_size = sizeof(main_gui_fragment_t)
};

static const lv_fragment_class_t color_select_cls = {
    .create_obj_cb = color_select_create_cb,
    .instance_size = sizeof(color_select_fragment_t)
};

static const lv_fragment_class_t mode_select_cls = {
    .create_obj_cb = mode_select_create_cb,
    .instance_size = sizeof(mode_select_fragment_t)
};

static const lv_fragment_class_t about_page_cls = {
    .create_obj_cb = about_page_create_cb,
    .instance_size = sizeof(about_page_fragment_t)
};


/* Callback functions */

static lv_obj_t * main_gui_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    main_gui_fragment_t* fragment = (main_gui_fragment_t*) self;
    lv_obj_add_event_cb(lv_scr_act(), return_gesture_cb, LV_EVENT_GESTURE, NULL);

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    lv_obj_t* list = lv_list_create(content);
    lv_obj_set_width(list, lv_pct(100));
    lv_obj_set_style_pad_row(list, 15, 0);
    lv_obj_set_style_pad_top(list, 10, 0);
    lv_obj_set_style_pad_bottom(list, 10, 0);
    lv_obj_set_style_pad_left(list, (240 - 120) / 2, 0);
    lv_obj_set_style_border_opa(list, 0, 0);
    lv_obj_set_style_bg_opa(list, 0, 0);

    fragment->pwr_btn = lv_btn_create(list);
    btn_set_text(fragment->pwr_btn, "OFF");
    lv_obj_set_style_bg_color(fragment->pwr_btn, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_add_event_cb(fragment->pwr_btn, pwr_btn_event_cb, LV_EVENT_CLICKED, fragment);

    fragment->color_btn = lv_btn_create(list);
    btn_set_text(fragment->color_btn, "COLOR");
    lv_obj_set_style_bg_color(fragment->color_btn, cur_color, 0);
    lv_obj_add_event_cb(fragment->color_btn, color_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(fragment->color_btn, color_btn_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_COLOR_CHANGED, fragment->color_btn, NULL);

    fragment->mode_btn = lv_btn_create(list);
    btn_set_text(fragment->mode_btn, "MODE");
    lv_obj_add_event_cb(fragment->mode_btn, mode_btn_event_cb, LV_EVENT_CLICKED, NULL);

    fragment->about_btn = lv_btn_create(list);
    btn_set_text(fragment->about_btn, "ABOUT");
    lv_obj_add_event_cb(fragment->about_btn, about_btn_event_cb, LV_EVENT_CLICKED, NULL);

    return content;
}

static lv_obj_t * color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    color_select_fragment_t* fragment = (color_select_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    fragment->color_selector = lv_colorwheel_create(content, true);
    lv_colorwheel_set_mode(fragment->color_selector, LV_COLORWHEEL_MODE_HUE);
    lv_colorwheel_set_mode_fixed(fragment->color_selector, true);
    lv_colorwheel_set_rgb(fragment->color_selector, cur_color);
    lv_obj_set_size(fragment->color_selector, lv_pct(75), lv_pct(75));
    lv_obj_center(fragment->color_selector);
    lv_obj_add_event_cb(fragment->color_selector, color_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);

    return content;
}

static lv_obj_t * mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    mode_select_fragment_t* fragment = (mode_select_fragment_t*) self;

    lv_obj_t* content = lv_btn_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    lv_obj_t* list = lv_list_create(content);
    lv_obj_set_width(list, lv_pct(100));
    lv_obj_set_style_pad_row(list, 15, 0);
    lv_obj_set_style_pad_top(list, 10, 0);
    lv_obj_set_style_pad_bottom(list, 10, 0);
    lv_obj_set_style_pad_left(list, (240 - 120) / 2, 0);
    lv_obj_set_style_border_opa(list, 0, 0);
    lv_obj_set_style_bg_opa(list, 0, 0);

    fragment->normal_mode = lv_btn_create(list);
    btn_set_text(fragment->normal_mode, "Normal");
    lv_obj_add_event_cb(fragment->normal_mode, mode_select_cb, LV_EVENT_CLICKED, fragment);

    fragment->breathing_mode = lv_btn_create(list);
    btn_set_text(fragment->breathing_mode, "Breathing");
    lv_obj_add_event_cb(fragment->breathing_mode, mode_select_cb, LV_EVENT_CLICKED, fragment);

    fragment->interval_mode = lv_btn_create(list);
    btn_set_text(fragment->interval_mode, "Lightbeam");
    lv_obj_add_event_cb(fragment->interval_mode, mode_select_cb, LV_EVENT_CLICKED, fragment);

    fragment->rainbow_mode = lv_btn_create(list);
    btn_set_text(fragment->rainbow_mode, "Rainbow");
    lv_obj_add_event_cb(fragment->rainbow_mode, mode_select_cb, LV_EVENT_CLICKED, fragment);

    switch (cur_mode) {
        case MODE_NORMAL:
            lv_obj_set_style_bg_color(fragment->normal_mode, lv_palette_main(LV_PALETTE_RED), 0);
            break;
        case MODE_BREATHING:
            lv_obj_set_style_bg_color(fragment->breathing_mode, lv_palette_main(LV_PALETTE_RED), 0);
            break;
        case MODE_LIGHTBEAM:
            lv_obj_set_style_bg_color(fragment->interval_mode, lv_palette_main(LV_PALETTE_RED), 0);
            break;
        case MODE_RAINBOW:
            lv_obj_set_style_bg_color(fragment->rainbow_mode, lv_palette_main(LV_PALETTE_RED), 0);
            break;
    }

    return content;
}

static lv_obj_t * about_page_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    about_page_fragment_t* fragment = (about_page_fragment_t*) self;

    lv_obj_t* content = lv_btn_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    return content;
}

static void container_del_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_DELETE)
    {
        lv_fragment_manager_del(manager);
    }
}

static void pwr_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        main_gui_fragment_t* fragment = (main_gui_fragment_t*) lv_event_get_user_data(e);
        pwr = ~pwr;
        if (pwr)
        {
            lv_obj_set_style_bg_color(fragment->pwr_btn, lv_palette_main(LV_PALETTE_GREEN), 0);
            lv_obj_t* label = lv_obj_get_child(fragment->pwr_btn, 0);
            lv_label_set_text(label, "ON");
        }
        else
        {
            lv_obj_set_style_bg_color(fragment->pwr_btn, lv_palette_main(LV_PALETTE_RED), 0);
            lv_obj_t* label = lv_obj_get_child(fragment->pwr_btn, 0);
            lv_label_set_text(label, "OFF");
        }
    }
}

static void color_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        color_select_fragment = lv_fragment_create(&color_select_cls, NULL);
        lv_fragment_manager_push(manager, color_select_fragment, &container);
    }
    else if (e->code == LV_EVENT_MSG_RECEIVED)
    {
        lv_obj_t* btn = (lv_obj_t*) lv_event_get_target(e);
        lv_msg_t* m = lv_event_get_msg(e);
        uint32_t id = lv_msg_get_id(m);
        printf("Received message\n");
        if (id == MSG_COLOR_CHANGED) {
            const msg_reply_t* reply = lv_msg_get_payload(m);
            if (reply->resp) {
                lv_obj_set_style_bg_color(btn, cur_color, 0);
                printf("Color btn style changed\n");
            }
            else {
                cur_color = pre_color;
            }
        }
    }
}

static void mode_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        mode_select_fragment = lv_fragment_create(&mode_select_cls, NULL);
        lv_fragment_manager_push(manager, mode_select_fragment, &container);
    }
}

static void about_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED) {
        about_page_fragment = lv_fragment_create(&about_page_cls, NULL);
        lv_fragment_manager_push(manager, about_page_fragment, &container);
    }
}

static void color_changed_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t* color_wheel = (lv_obj_t*) lv_event_get_target(e);

        pre_color = cur_color;
        cur_color = lv_colorwheel_get_rgb(color_wheel);
        printf("Color selected: #%04x\n", lv_color_to16(cur_color));

        msg_struct_t data;
        data.msg = MSG_COLOR_CHANGED;
        data.mode = cur_mode;
        if (cur_mode == MODE_NORMAL) {
            data.setting.normal.color = lv_color_to16(cur_color);
        }
        else if (cur_mode == MODE_BREATHING) {
            data.setting.breathing.color = lv_color_to16(cur_color);
        }
        else if (cur_mode == MODE_LIGHTBEAM) {
            data.setting.lightbeam.color = lv_color_to16(cur_color);
        }
        queue_push(data);
    }
}

static void return_gesture_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_RIGHT && lv_fragment_manager_get_stack_size(manager) > 1) {
            lv_fragment_manager_pop(manager);
        }
    }
}

static void mode_select_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* btn = lv_event_get_target(e);
        mode_select_fragment_t* fragment = (mode_select_fragment_t*) lv_event_get_user_data(e);

        led_mode_t tar_mode = MODE_NORMAL;
        if (btn == fragment->normal_mode) {
            tar_mode = MODE_NORMAL;
        }
        else if (btn == fragment->breathing_mode) {
            tar_mode = MODE_BREATHING;
        }
        else if (btn == fragment->interval_mode) {
            tar_mode = MODE_LIGHTBEAM;
        }
        else if (btn == fragment->rainbow_mode) {
            tar_mode = MODE_RAINBOW;
        }

        if (tar_mode == cur_mode) return;
        pre_mode = cur_mode;
        cur_mode = tar_mode;
        switch (pre_mode) {
            case MODE_NORMAL:
                lv_obj_set_style_bg_color(fragment->normal_mode, lv_palette_main(LV_PALETTE_BLUE), 0);
                break;
            case MODE_BREATHING:
                lv_obj_set_style_bg_color(fragment->breathing_mode, lv_palette_main(LV_PALETTE_BLUE), 0);
                break;
            case MODE_LIGHTBEAM:
                lv_obj_set_style_bg_color(fragment->interval_mode, lv_palette_main(LV_PALETTE_BLUE), 0);
                break;
            case MODE_RAINBOW:
                lv_obj_set_style_bg_color(fragment->rainbow_mode, lv_palette_main(LV_PALETTE_BLUE), 0);
                break;
        }
        switch (cur_mode) {
            case MODE_NORMAL:
                lv_obj_set_style_bg_color(fragment->normal_mode, lv_palette_main(LV_PALETTE_RED), 0);
                break;
            case MODE_BREATHING:
                lv_obj_set_style_bg_color(fragment->breathing_mode, lv_palette_main(LV_PALETTE_RED), 0);
                break;
            case MODE_LIGHTBEAM:
                lv_obj_set_style_bg_color(fragment->interval_mode, lv_palette_main(LV_PALETTE_RED), 0);
                break;
            case MODE_RAINBOW:
                lv_obj_set_style_bg_color(fragment->rainbow_mode, lv_palette_main(LV_PALETTE_RED), 0);
                break;
        }
    }
}


/* Style functions */

static void btn_set_text(lv_obj_t* btn, const char* text)
{
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_set_size(btn, 120, 50);
    lv_obj_center(btn);
}


/* Main function */

void create_main_gui(void)
{
    cur_color = lv_palette_main(LV_PALETTE_BLUE);

    container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_bg_opa(container, 0, 0);
    lv_obj_set_style_border_opa(container, 0, 0);

    manager = lv_fragment_manager_create(NULL);
    lv_obj_add_event_cb(container, container_del_cb, LV_EVENT_DELETE, NULL);

    main_gui_fragment = lv_fragment_create(&main_gui_cls, NULL);
    lv_fragment_manager_push(manager, main_gui_fragment, &container);
}
