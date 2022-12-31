#include "main_gui.h"

configuration_t configuration;


/* Static variables define */

static lv_obj_t* container;
static lv_fragment_manager_t* manager;


/* Static functions define */

static lv_obj_t* main_gui_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* about_page_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static void main_gui_constructor_cb(lv_fragment_t* self, void* args);
static void color_select_construct_cb(lv_fragment_t* self, void* args);
static void container_del_cb(lv_event_t* e);
static void pwr_btn_event_cb(lv_event_t* e);
static void color_btn_event_cb(lv_event_t* e);
static void mode_btn_event_cb(lv_event_t* e);
static void about_btn_event_cb(lv_event_t* e);
static void color_selector_event_cb(lv_event_t* e);
static void return_gesture_cb(lv_event_t* e);
static void mode_changed_cb(lv_event_t* e);
static void message_received_cb(void* s, lv_msg_t* m);

static void btn_set_text(lv_obj_t* btn, const char* text);
static void* init_message_package(void);


/* Static structs define */

static const lv_fragment_class_t main_gui_cls = {
    .create_obj_cb = main_gui_create_cb,
    .constructor_cb = main_gui_constructor_cb,
    .instance_size = sizeof(main_gui_fragment_t)
};

static const lv_fragment_class_t color_select_cls = {
    .create_obj_cb = color_select_create_cb,
    .constructor_cb = color_select_construct_cb,
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

    lv_obj_t* pwr_btn = lv_btn_create(list);
    btn_set_text(pwr_btn, "OFF");
    lv_obj_set_style_bg_color(pwr_btn, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_add_event_cb(pwr_btn, pwr_btn_event_cb, LV_EVENT_CLICKED, &fragment->power);

    lv_obj_t* color_btn = lv_btn_create(list);
    btn_set_text(color_btn, "COLOR");
    lv_obj_add_event_cb(color_btn, color_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* mode_btn = lv_btn_create(list);
    btn_set_text(mode_btn, "MODE");
    lv_obj_add_event_cb(mode_btn, mode_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* about_btn = lv_btn_create(list);
    btn_set_text(about_btn, "ABOUT");
    lv_obj_add_event_cb(about_btn, about_btn_event_cb, LV_EVENT_CLICKED, NULL);

    return content;
}

static lv_obj_t * color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    color_select_fragment_t* fragment = (color_select_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    lv_obj_t* color_selector = lv_colorwheel_create(content, true);
    lv_colorwheel_set_mode(color_selector, LV_COLORWHEEL_MODE_HUE);
    lv_colorwheel_set_mode_fixed(color_selector, true);
    // if (configuration.mode == MODE_NORMAL) lv_colorwheel_set_rgb(color_selector, configuration.setting.normal.color);
    // else if (configuration.mode == MODE_BREATHING) lv_colorwheel_set_rgb(color_selector, configuration.setting.breathing.color);
    // else if (configuration.mode == MODE_LIGHTBEAM) lv_colorwheel_set_rgb(color_selector, configuration.setting.lightbeam.color);
    lv_obj_set_size(color_selector, lv_pct(75), lv_pct(75));
    lv_obj_center(color_selector);
    lv_obj_add_event_cb(color_selector, color_selector_event_cb, LV_EVENT_VALUE_CHANGED, fragment);
    lv_msg_subscribe(RES_CONFIG_UPDATED, message_received_cb, fragment);

    return content;
}

static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
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

    lv_obj_t* btngroup = btngroup_create();

    lv_obj_t* normal_btn = lv_btn_create(list);
    btn_set_text(normal_btn, "Normal");
    btngroup_add_btn(btngroup, normal_btn);
    btngroup_set_checked(btngroup, 0);

    lv_obj_t* breathing_btn = lv_btn_create(list);
    btn_set_text(breathing_btn, "Breathing");
    btngroup_add_btn(btngroup, breathing_btn);

    lv_obj_t* lightbeam_btn = lv_btn_create(list);
    btn_set_text(lightbeam_btn, "Lightbeam");
    btngroup_add_btn(btngroup, lightbeam_btn);

    lv_obj_t* rainbow_btn = lv_btn_create(list);
    btn_set_text(rainbow_btn, "Rainbow");
    btngroup_add_btn(btngroup, rainbow_btn);

    lv_obj_add_event_cb(btngroup, mode_changed_cb, EVENT_SELECTED_CHANGED, NULL);

    return content;
}

static lv_obj_t * about_page_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    about_page_fragment_t* fragment = (about_page_fragment_t*) self;

    lv_obj_t* content = lv_btn_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    return content;
}

static void main_gui_constructor_cb(lv_fragment_t* self, void* args) {
    LV_UNUSED(args);
    main_gui_fragment_t* fragment = (main_gui_fragment_t*) self;
    fragment->power = 0;
}

static void color_select_construct_cb(lv_fragment_t* self, void* args) {
    LV_UNUSED(args);
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
        lv_obj_t* btn = (lv_obj_t*) lv_event_get_target(e);
        uint8_t* pwr = (uint8_t*) lv_event_get_user_data(e);
        *(pwr) = ~(*pwr);
        if (*pwr)
        {
            lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREEN), 0);
            lv_obj_t* label = lv_obj_get_child(btn, 0);
            lv_label_set_text(label, "ON");
        }
        else
        {
            lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);
            lv_obj_t* label = lv_obj_get_child(btn, 0);
            lv_label_set_text(label, "OFF");
        }
    }
}

static void color_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        lv_obj_t* obj = lv_event_get_target(e);
        lv_fragment_t* color_select_fragment = lv_fragment_create(&color_select_cls, obj);
        lv_fragment_manager_push(manager, color_select_fragment, &container);
    }
}

static void mode_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        lv_fragment_t* mode_select_fragment = lv_fragment_create(&mode_select_cls, NULL);
        lv_fragment_manager_push(manager, mode_select_fragment, &container);
    }
}

static void about_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED) {
        lv_fragment_t* about_page_fragment = lv_fragment_create(&about_page_cls, NULL);
        lv_fragment_manager_push(manager, about_page_fragment, &container);
    }
}

static void color_selector_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t* color_wheel = lv_event_get_target(e);
        color_select_fragment_t* fragment = (color_select_fragment_t*) lv_event_get_user_data(e);

        lv_color_t color = lv_colorwheel_get_rgb(color_wheel);
        printf("color: #%04x\n", lv_color_to16(color));
        if (configuration.mode == MODE_NORMAL) configuration.setting.normal.color = color;
        else if (configuration.mode == MODE_BREATHING) configuration.setting.breathing.color = color;
        else if (configuration.mode == MODE_LIGHTBEAM) configuration.setting.lightbeam.color = color;

        void* data = init_message_package();
        msg_send(MSG_WRITE_CONFIG, data);
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

static void mode_changed_cb(lv_event_t* e) {
    if (e->code == EVENT_SELECTED_CHANGED) {
        btngroup_t* tar = lv_event_get_param(e);
        printf("pre_sel: %d, cur_sel: %d", tar->pre_sel, tar->cur_sel);
    }
}

static void message_received_cb(void* s, lv_msg_t* m) {
    LV_UNUSED(s);
    uint32_t id = lv_msg_get_id(m);
    const msg_reply_t* reply = lv_msg_get_payload(m);
    if (id == RES_CONFIG_UPDATED) {
        printf("GUI received message, id: %d, result: %d\n", id, reply->resp);
        if (reply->resp) {
            refresh_main_gui();
            printf("GUI refreshed\n");
        }
    }
}


/* Utility functions */

static void btn_set_text(lv_obj_t* btn, const char* text)
{
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_set_size(btn, 120, 50);
    lv_obj_center(btn);
}

static void* init_message_package(void) {
    configuration_t* data = (configuration_t*) heap_caps_malloc(sizeof(configuration_t), MALLOC_CAP_DEFAULT);
    memcpy(data, &configuration, sizeof(configuration_t));
    return data;
}


/* Global function */

void create_main_gui(void)
{
    container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_style_bg_opa(container, 0, 0);
    lv_obj_set_style_border_opa(container, 0, 0);

    manager = lv_fragment_manager_create(NULL);
    lv_obj_add_event_cb(container, container_del_cb, LV_EVENT_DELETE, NULL);

    lv_fragment_t* main_gui_fragment = lv_fragment_create(&main_gui_cls, NULL);
    lv_fragment_manager_push(manager, main_gui_fragment, &container);
}

void refresh_main_gui(void) {

}
