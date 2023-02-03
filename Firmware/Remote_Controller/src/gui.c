#include "gui.h"

/* Static variables define */

static lv_obj_t* container;
static lv_fragment_manager_t* manager;

/* Static functions define */

static lv_obj_t* home_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* about_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* breathing_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* lightbeam_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* rainbow_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* wifi_scan_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t* wifi_connect_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static void wifi_scan_construct_cb(lv_fragment_t* self, void* args);
static void wifi_connect_construct_cb(lv_fragment_t* self, void* args);
static void container_del_cb(lv_event_t* e);
static void pwr_btn_event_cb(lv_event_t* e);
static void color_btn_event_cb(lv_event_t* e);
static void mode_btn_event_cb(lv_event_t* e);
static void about_btn_event_cb(lv_event_t* e);
static void color_selector_event_cb(lv_event_t* e);
static void gesture_event_cb(lv_event_t* e);
static void mode_changed_cb(lv_event_t* e);
static void setting_btn_event_cb(lv_event_t* e);
static void confirm_panel_event_cb(lv_event_t* e);
static void wifi_list_item_event_cb(lv_event_t* e);
static void password_area_event_cb(lv_event_t* e);
static void connect_btn_event_cb(lv_event_t* e);
static void keyboard_event_cb(lv_event_t* e);
static void refresh_btn_event_cb(lv_event_t* e);
static void message_received_cb(void* s, lv_msg_t* m);

static void stylish_default_btn(lv_obj_t* btn, const char* text);
static lv_obj_t* create_mode_select_item(lv_obj_t* parent, const char* text);
static void stylish_setting_btn(lv_obj_t* btn);
static void power_btn_set_state(lv_obj_t* btn, bool state);

static void* init_message_package(void);


/* Static structs define */

static const lv_fragment_class_t home_cls = {
    .create_obj_cb = home_create_cb,
    .instance_size = sizeof(home_fragment_t)
};

static const lv_fragment_class_t color_select_cls = {
    .create_obj_cb = color_select_create_cb,
    .instance_size = sizeof(color_select_fragment_t)
};

static const lv_fragment_class_t mode_select_cls = {
    .create_obj_cb = mode_select_create_cb,
    .instance_size = sizeof(mode_select_fragment_t)
};

static const lv_fragment_class_t about_cls = {
    .create_obj_cb = about_create_cb,
    .instance_size = sizeof(about_fragment_t)
};

static const lv_fragment_class_t breathing_setting_cls = {
    .create_obj_cb = breathing_mode_setting_create_cb,
    .instance_size = sizeof(breathing_setting_fragment_t)
};

static const lv_fragment_class_t lightbeam_setting_cls = {
    .create_obj_cb = lightbeam_mode_setting_create_cb,
    .instance_size = sizeof(lightbeam_setting_fragment_t)
};

static const lv_fragment_class_t rainbow_setting_cls = {
    .create_obj_cb = rainbow_mode_setting_create_cb,
    .instance_size = sizeof(rainbow_setting_fragment_t)
};

static const lv_fragment_class_t wifi_scan_cls = {
    .create_obj_cb = wifi_scan_create_cb,
    .constructor_cb = wifi_scan_construct_cb,
    .instance_size = sizeof(wifi_scan_fragment_t)
};

static const lv_fragment_class_t wifi_connect_cls = {
    .constructor_cb = wifi_connect_construct_cb,
    .create_obj_cb = wifi_connect_create_cb,
    .instance_size = sizeof(wifi_connect_fragment_t)
};


/* Callback functions */

static lv_obj_t* home_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    home_fragment_t* fragment = (home_fragment_t*) self;

    lv_obj_t* content = lv_list_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* pwr_btn = lv_btn_create(content);
    lv_obj_add_flag(pwr_btn, LV_OBJ_FLAG_CHECKABLE);
    stylish_default_btn(pwr_btn, "");
    power_btn_set_state(pwr_btn, configuration.power);
    lv_obj_add_event_cb(pwr_btn, pwr_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* color_btn = lv_btn_create(content);
    stylish_default_btn(color_btn, "COLOR");
    lv_obj_set_style_bg_color(color_btn, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_add_event_cb(color_btn, color_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* mode_btn = lv_btn_create(content);
    stylish_default_btn(mode_btn, "MODE");
    lv_obj_add_event_cb(mode_btn, mode_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* about_btn = lv_btn_create(content);
    stylish_default_btn(about_btn, "ABOUT");
    lv_obj_add_event_cb(about_btn, about_btn_event_cb, LV_EVENT_CLICKED, NULL);

    return content;
}

static lv_obj_t* color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    color_select_fragment_t* fragment = (color_select_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    lv_obj_t* color_selector = lv_colorwheel_create(content, true);
    lv_colorwheel_set_mode(color_selector, LV_COLORWHEEL_MODE_HUE);
    lv_colorwheel_set_mode_fixed(color_selector, true);
    if (configuration.mode == MODE_NORMAL) lv_colorwheel_set_rgb(color_selector, configuration.setting.normal.color);
    else if (configuration.mode == MODE_BREATHING) lv_colorwheel_set_rgb(color_selector, configuration.setting.breathing.color);
    else if (configuration.mode == MODE_LIGHTBEAM) lv_colorwheel_set_rgb(color_selector, configuration.setting.lightbeam.color);
    lv_obj_set_size(color_selector, lv_pct(75), lv_pct(75));
    lv_obj_center(color_selector);
    lv_obj_add_event_cb(color_selector, color_selector_event_cb, LV_EVENT_VALUE_CHANGED, fragment);

    return content;
}

static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    mode_select_fragment_t* fragment = (mode_select_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* checked_group = checkedgroup_create();

    lv_obj_t* normal_btn = create_mode_select_item(content, "Normal");
    checkedgroup_add_item(checked_group, normal_btn);
    checkedgroup_set_right_btn_event_cb(checked_group, 0, setting_btn_event_cb);

    lv_obj_t* breathing_btn = create_mode_select_item(content, "Breathing");
    checkedgroup_add_item(checked_group, breathing_btn);
    checkedgroup_set_right_btn_event_cb(checked_group, 1, setting_btn_event_cb);

    lv_obj_t* lightbeam_btn = create_mode_select_item(content, "Lightbeam");
    checkedgroup_add_item(checked_group, lightbeam_btn);
    checkedgroup_set_right_btn_event_cb(checked_group, 2, setting_btn_event_cb);

    lv_obj_t* rainbow_btn = create_mode_select_item(content, "Rainbow");
    checkedgroup_add_item(checked_group, rainbow_btn);
    checkedgroup_set_right_btn_event_cb(checked_group, 3, setting_btn_event_cb);

    checkedgroup_set_checked(checked_group, configuration.mode);

    lv_obj_add_event_cb(checked_group, mode_changed_cb, EVENT_SELECTED_CHANGED, NULL);

    return content;
}

static lv_obj_t* about_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    about_fragment_t* fragment = (about_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* product_name_label = lv_label_create(content);
    lv_label_set_text(product_name_label, "Product name:");

    lv_obj_t* product_name = lv_label_create(content);
    lv_label_set_text(product_name, "SmartLight Controller");

    lv_obj_t* firmware_version_label = lv_label_create(content);
    lv_label_set_text(firmware_version_label, "Firmware version:");

    lv_obj_t* firmware_version = lv_label_create(content);
    lv_label_set_text(firmware_version, "v0.1");

    lv_obj_t* author_label = lv_label_create(content);
    lv_label_set_text(author_label, "Author:");

    lv_obj_t* author_name = lv_label_create(content);
    lv_label_set_text(author_name, "Jackie");

    LV_IMG_DECLARE(img_author_avatar);
    lv_obj_t* avatar = lv_img_create(content);
    lv_img_set_src(avatar, &img_author_avatar);
    lv_obj_set_size(avatar, 150, 150);

    return content;
}

static lv_obj_t* breathing_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    breathing_setting_fragment_t* fragment = (breathing_setting_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 5, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* duration_label = lv_label_create(content);
    lv_label_set_text(duration_label, "Duration:");

    fragment->duration_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->duration_selector, 0, 5000);
    styled_spinbox_set_step(fragment->duration_selector, 500);
    styled_spinbox_set_value(fragment->duration_selector, configuration.setting.breathing.duration);

    lv_obj_t* interval_label = lv_label_create(content);
    lv_label_set_text(interval_label, "Interval:");

    fragment->interval_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->interval_selector, 0, 5000);
    styled_spinbox_set_step(fragment->interval_selector, 500);
    styled_spinbox_set_value(fragment->interval_selector, configuration.setting.breathing.interval);

    static led_mode_t breathing_mode_tag = MODE_BREATHING;
    lv_obj_t* confirm_panel = create_confirm_panel(content);
    lv_obj_set_user_data(confirm_panel, &breathing_mode_tag);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_APPLIED, fragment);

    return content;
}

static lv_obj_t* lightbeam_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    lightbeam_setting_fragment_t* fragment = (lightbeam_setting_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 5, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* len_label = lv_label_create(content);
    lv_label_set_text(len_label, "Len:");

    fragment->len_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->len_selector, 0, 50);
    styled_spinbox_set_step(fragment->len_selector, 1);
    styled_spinbox_set_value(fragment->len_selector, configuration.setting.lightbeam.len);

    lv_obj_t* gap_label = lv_label_create(content);
    lv_label_set_text(gap_label, "Interval:");

    fragment->gap_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->gap_selector, 0, 50);
    styled_spinbox_set_step(fragment->gap_selector, 1);
    styled_spinbox_set_value(fragment->gap_selector, configuration.setting.lightbeam.gap);

    lv_obj_t* speed_label = lv_label_create(content);
    lv_label_set_text(speed_label, "Speed:");

    fragment->speed_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->speed_selector, 0, 100);
    styled_spinbox_set_step(fragment->speed_selector, 1);
    styled_spinbox_set_value(fragment->speed_selector, configuration.setting.lightbeam.speed);

    lv_obj_t* direction_label = lv_label_create(content);
    lv_label_set_text(direction_label, "Direction:");

    fragment->direction_selector = lv_btnmatrix_create(content);
    static const char* dir_map[] = {"Left", "Right", ""};
    lv_btnmatrix_set_map(fragment->direction_selector, dir_map);
    lv_btnmatrix_set_btn_ctrl_all(fragment->direction_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(fragment->direction_selector, true);
    lv_btnmatrix_set_btn_ctrl(fragment->direction_selector, 0, LV_BTNMATRIX_CTRL_CHECKED);
    lv_obj_set_size(fragment->direction_selector, 120, 50);
    lv_obj_set_style_pad_all(fragment->direction_selector, 0, 0);
    lv_btnmatrix_set_btn_ctrl(fragment->direction_selector, configuration.setting.lightbeam.dir, LV_BTNMATRIX_CTRL_CHECKED);

    lv_obj_t* faded_end_label = lv_label_create(content);
    lv_label_set_text(faded_end_label, "Faded end:");

    fragment->faded_end_selector = lv_btnmatrix_create(content);
    static const char* faded_end_map[] = {"Head", "Tail", ""};
    lv_btnmatrix_set_map(fragment->faded_end_selector, faded_end_map);
    lv_btnmatrix_set_btn_ctrl_all(fragment->faded_end_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_obj_set_size(fragment->faded_end_selector, 120, 50);
    lv_obj_set_style_pad_all(fragment->faded_end_selector, 0, 0);
    if (configuration.setting.lightbeam.faded_end.FADED_HEAD)
        lv_btnmatrix_set_btn_ctrl(fragment->faded_end_selector, 0, LV_BTNMATRIX_CTRL_CHECKED);
    if (configuration.setting.lightbeam.faded_end.FADED_TAIL)
        lv_btnmatrix_set_btn_ctrl(fragment->faded_end_selector, 1, LV_BTNMATRIX_CTRL_CHECKED);

    lv_obj_t* head_len_label = lv_label_create(content);
    lv_label_set_text(head_len_label, "Head len:");

    fragment->head_len_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->head_len_selector, 0, 50);
    styled_spinbox_set_step(fragment->head_len_selector, 1);
    styled_spinbox_set_value(fragment->head_len_selector, configuration.setting.lightbeam.head_len);

    lv_obj_t* tail_len_label = lv_label_create(content);
    lv_label_set_text(tail_len_label, "Head len:");

    fragment->tail_len_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->tail_len_selector, 0, 50);
    styled_spinbox_set_step(fragment->tail_len_selector, 1);
    styled_spinbox_set_value(fragment->tail_len_selector, configuration.setting.lightbeam.tail_len);

    static led_mode_t lightbeam_mode_tag = MODE_LIGHTBEAM;
    lv_obj_t* confirm_panel = create_confirm_panel(content);
    lv_obj_set_user_data(confirm_panel, &lightbeam_mode_tag);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_APPLIED, fragment);

    return content;
}

static lv_obj_t* rainbow_mode_setting_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    rainbow_setting_fragment_t* fragment = (rainbow_setting_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 5, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* speed_label = lv_label_create(content);
    lv_label_set_text(speed_label, "Speed:");

    fragment->speed_selector = create_styled_spinbox(content);
    styled_spinbox_set_range(fragment->speed_selector, 0, 100);
    styled_spinbox_set_step(fragment->speed_selector, 1);
    styled_spinbox_set_value(fragment->speed_selector, configuration.setting.rainbow.speed);

    static led_mode_t rainbow_mode_tag = MODE_RAINBOW;
    lv_obj_t* confirm_panel = create_confirm_panel(content);
    lv_obj_set_user_data(confirm_panel, &rainbow_mode_tag);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_APPLIED, fragment);

    return content;
}

static lv_obj_t* wifi_scan_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    wifi_scan_fragment_t* fragment = (wifi_scan_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 0, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    if (fragment->wifi_list) {
        struct list_elem_t* p = fragment->wifi_list->head;
        for (uint8_t i = 0; i < fragment->wifi_list->size; ++i, p = p->next) {
            wifi_info_t* info = (wifi_info_t*) p->data;
            lv_obj_t* item = create_wifi_list_item(content, info->ssid);
            lv_obj_add_event_cb(item, wifi_list_item_event_cb, LV_EVENT_CLICKED, NULL);
        }
    }

    lv_obj_t* refresh_btn = lv_btn_create(content);
    stylish_default_btn(refresh_btn, "Refresh");
    lv_obj_add_event_cb(refresh_btn, refresh_btn_event_cb, LV_EVENT_CLICKED, NULL);

    return content;
}

static lv_obj_t* wifi_connect_create_cb(lv_fragment_t* self, lv_obj_t* parent) {
    wifi_connect_fragment_t* fragment = (wifi_connect_fragment_t*) self;

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 20, 0);
    lv_obj_set_style_pad_top(content, 10, 0);
    lv_obj_set_style_pad_bottom(content, 10, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_set_width(keyboard, lv_pct(100));
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_keyboard_set_textarea(keyboard, NULL);
    lv_keyboard_set_popovers(keyboard, true);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_FLOATING);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t* ssid_label = lv_label_create(content);
    lv_obj_set_size(ssid_label, lv_pct(100), 30);
    lv_obj_align(ssid_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(ssid_label, fragment->ssid);
    lv_obj_set_style_text_font(ssid_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_border_side(ssid_label, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_opa(ssid_label, lv_pct(50), 0);
    lv_obj_set_style_border_width(ssid_label, 1, 0);
    lv_obj_set_style_border_color(ssid_label, lv_palette_main(LV_PALETTE_GREY), 0);

    lv_obj_t* password = lv_textarea_create(content);
    lv_textarea_set_placeholder_text(password, "Password");
    lv_textarea_set_one_line(password, true);
    lv_textarea_set_password_mode(password, true);
    lv_obj_add_event_cb(password, password_area_event_cb, LV_EVENT_ALL, keyboard);
    lv_obj_set_size(password, 200, 40);

    lv_obj_t* connect_btn = lv_btn_create(content);
    lv_obj_add_event_cb(connect_btn, connect_btn_event_cb, LV_EVENT_CLICKED, password);
    stylish_default_btn(connect_btn, "Connect");
    lv_obj_set_style_translate_x(connect_btn, (240 - 120 - 20 * 2) / 2, 0);

    return content;
}

static void wifi_scan_construct_cb(lv_fragment_t* self, void* args) {
    wifi_scan_fragment_t* fragment = (wifi_scan_fragment_t*) self;
    fragment->wifi_list = NULL;

    LV_UNUSED(args);
}

static void wifi_connect_construct_cb(lv_fragment_t* self, void* args) {
    wifi_connect_fragment_t* fragment = (wifi_connect_fragment_t*) self;

    fragment->ssid = (char*) args;
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
        lv_obj_t* btn = lv_event_get_target(e);

        void* data = init_message_package();
        msg_request_t request;
        request.msg = MSG_WRITE_HOST;
        request.user_data = data;
        xQueueSend(messageHandler, &request, 1000);

        configuration.power = !configuration.power;
    }
}

static void color_btn_event_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_CLICKED)
    {
        lv_fragment_t* color_select_fragment = lv_fragment_create(&color_select_cls, NULL);
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
        lv_fragment_t* about_fragment = lv_fragment_create(&about_cls, NULL);
        lv_fragment_manager_push(manager, about_fragment, &container);
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

        void* data = init_message_package();

        if (configuration.mode == MODE_NORMAL) configuration.setting.normal.color = color;
        else if (configuration.mode == MODE_BREATHING) configuration.setting.breathing.color = color;
        else if (configuration.mode == MODE_LIGHTBEAM) configuration.setting.lightbeam.color = color;

        msg_request_t request;
        request.msg = MSG_WRITE_HOST;
        request.user_data = data;
        xQueueSend(messageHandler, &request, 1000);
    }
}

static void gesture_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_RIGHT && lv_fragment_manager_get_stack_size(manager) > 1) {
            lv_fragment_manager_pop(manager);
        }
    }
}

static void mode_changed_cb(lv_event_t* e) {
    if (e->code == EVENT_SELECTED_CHANGED) {
        uint8_t* cur_sel = (uint8_t*) lv_event_get_param(e);
        printf("cur_sel: %d\n", *cur_sel);

        void* data = init_message_package();

        if (*cur_sel == MODE_NORMAL)
        {
            configuration.mode = MODE_NORMAL;
        }
        else if (*cur_sel == MODE_BREATHING) {
            configuration.mode = MODE_BREATHING;
        }
        else if (*cur_sel == MODE_LIGHTBEAM)
        {
            configuration.mode = MODE_LIGHTBEAM;
        }
        else if (*cur_sel = MODE_RAINBOW) {
            configuration.mode = MODE_RAINBOW;
        }

        msg_request_t request;
        request.msg = MSG_WRITE_HOST;
        request.user_data = data;
        xQueueSend(messageHandler, &request, 1000);
    }
}

static void confirm_panel_event_cb(lv_event_t* e) {
    if (e->code == EVENT_CONFIRMED || e->code == EVENT_APPLIED) {
        lv_obj_t* obj = lv_event_get_target(e);
        led_mode_t* mode = (led_mode_t*) lv_obj_get_user_data(obj);
        void* data = init_message_package();
        if (*mode == MODE_BREATHING) {
            breathing_setting_fragment_t* fragment = (breathing_setting_fragment_t*) lv_event_get_user_data(e);

            configuration.setting.breathing.duration = styled_spinbox_get_value(fragment->duration_selector);
            configuration.setting.breathing.interval = styled_spinbox_get_value(fragment->interval_selector);
        }
        else if (*mode == MODE_LIGHTBEAM) {
            lightbeam_setting_fragment_t* fragment = (lightbeam_setting_fragment_t*) lv_event_get_user_data(e);

            configuration.setting.lightbeam.len = styled_spinbox_get_value(fragment->len_selector);
            configuration.setting.lightbeam.gap = styled_spinbox_get_value(fragment->gap_selector);
            configuration.setting.lightbeam.speed = styled_spinbox_get_value(fragment->speed_selector);
            configuration.setting.lightbeam.head_len = styled_spinbox_get_value(fragment->head_len_selector);
            configuration.setting.lightbeam.tail_len = styled_spinbox_get_value(fragment->tail_len_selector);
            uint16_t selected = lv_btnmatrix_get_selected_btn(fragment->direction_selector);
            if (selected == MOVE_LEFT) configuration.setting.lightbeam.dir = MOVE_LEFT;
            else if (selected == MOVE_RIGHT) configuration.setting.lightbeam.dir = MOVE_RIGHT;
            faded_end_t faded_end;
            if (lv_btnmatrix_has_btn_ctrl(fragment->faded_end_selector, 0, LV_BTNMATRIX_CTRL_CHECKED)) faded_end.FADED_HEAD = 1;
            if (lv_btnmatrix_has_btn_ctrl(fragment->faded_end_selector, 1, LV_BTNMATRIX_CTRL_CHECKED)) faded_end.FADED_TAIL = 1;
            configuration.setting.lightbeam.faded_end = faded_end;
        }
        else if (*mode == MODE_RAINBOW) {
            rainbow_setting_fragment_t* fragment = (rainbow_setting_fragment_t*) lv_event_get_user_data(e);

            configuration.setting.rainbow.speed = styled_spinbox_get_value(fragment->speed_selector);
        }
        msg_request_t request;
        request.msg = MSG_WRITE_HOST;
        request.user_data = data;
        xQueueSend(messageHandler, &request, 1000);
    }
    if (e->code == EVENT_CONFIRMED) {
        lv_fragment_manager_pop(manager);
    }
}

static void setting_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* checked_group = (lv_obj_t*) lv_event_get_user_data(e);
        lv_obj_t* item = lv_obj_get_parent(lv_event_get_target(e));
        uint8_t index = checkedgroup_get_index(checked_group, item);
        lv_fragment_t* fragment = NULL;
        if (index == 1) {
            fragment = lv_fragment_create(&breathing_setting_cls, NULL);
        }
        else if (index == 2) {
            fragment = lv_fragment_create(&lightbeam_setting_cls, NULL);
        }
        else if (index == 3) {
            fragment = lv_fragment_create(&rainbow_setting_cls, NULL);
        }
        if (fragment) lv_fragment_manager_push(manager, fragment, &container);
    }
}

static void wifi_list_item_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* item = lv_event_get_target(e);
        char* ssid = lv_obj_get_user_data(item);
        lv_fragment_t* connect_page = lv_fragment_create(&wifi_connect_cls, ssid);
        lv_fragment_manager_push(manager, connect_page, &container);
    }
}

static void password_area_event_cb(lv_event_t* e) {
    lv_obj_t* textarea = lv_event_get_target(e);
    lv_obj_t* keyboard = lv_event_get_user_data(e);
    if (e->code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(keyboard, textarea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(keyboard);
    }
    else if (e->code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(keyboard);
        
    }
}

static void connect_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        lv_obj_t* psw_area = lv_event_get_user_data(e);
        wifi_connect_fragment_t* fragment = (wifi_connect_fragment_t*) lv_fragment_manager_get_top(manager);
        wifi_connect_t* conn = (wifi_connect_t*) malloc(sizeof(wifi_connect_t));
        strcpy(conn->ssid, fragment->ssid);
        strcpy(conn->password, lv_textarea_get_text(psw_area));

        msg_request_t request;
        request.msg = MSG_WIFI_CONNECT;
        request.user_data = conn;
        xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);

        show_loading_gui("Connecting...");
    }
}

static void keyboard_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_READY || e->code == LV_EVENT_CANCEL) {
        lv_obj_t* keyboard = lv_event_get_target(e);
        lv_obj_t* textarea = lv_keyboard_get_textarea(keyboard);
        lv_obj_clear_state(textarea, LV_STATE_FOCUSED);
        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(keyboard);
    }
}

static void refresh_btn_event_cb(lv_event_t* e) {
    if (e->code == LV_EVENT_CLICKED) {
        show_loading_gui("Scaning...");
        msg_request_t request;
        request.msg = MSG_WIFI_SCAN;
        request.user_data = NULL;
        xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
    }
}

static void message_received_cb(void* s, lv_msg_t* m) {
    LV_UNUSED(s);
    uint32_t id = lv_msg_get_id(m);
    if (id == REFRESH_GUI) {
        refresh_gui();
        printf("GUI refreshed\n");
    }
    else if (id == MSG_WIFI_SCAN_DONE) {
        if (lv_fragment_manager_get_stack_size(manager) == 0) show_connect_gui();
        wifi_scan_fragment_t* fragment = (wifi_scan_fragment_t*) lv_fragment_manager_get_top(manager);
        wifi_list_t* list = *((wifi_list_t**)lv_msg_get_payload(m));
        if (fragment->wifi_list) {
            for (struct list_elem_t* p = fragment->wifi_list->head; p; p = p->next) {
                free(p->data);
                p->data = NULL;
            }
            list_del(fragment->wifi_list);
        }
        fragment->wifi_list = list;

        refresh_gui();

        // disable loading gui
        hide_loading_gui();
    }
    else if (id == MSG_WIFI_CONNECTED) {
        hide_loading_gui();
        lv_fragment_manager_pop(manager);
    }
    else if (id == MSG_MATCH_RESULT) {
        msg_reply_t* reply = (msg_reply_t*) lv_msg_get_payload(m);

        if (reply->resp) {
            loading_gui_set_text("Requiring data...");

            msg_request_t request;
            request.msg = MSG_READ_HOST;
            request.user_data = NULL;
            xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
        }
        else {
            hide_loading_gui();
            show_matching_failed_gui();
        }
    }
    else if (id == MSG_READ_RESULT) {
        msg_reply_t* reply = (msg_reply_t*) lv_msg_get_payload(m);

        if (reply->resp) {

        }
        else {

        }
    }
    else if (id == MSG_WRITE_RESULT) {
        msg_reply_t* reply = (msg_reply_t*) lv_msg_get_payload(m);

        if (reply->resp) {

        }
        else {

        }
    }
}


/* Style functions */

static void stylish_default_btn(lv_obj_t* btn, const char* text)
{
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_set_size(btn, BTN_HOR_RES, BTN_VER_RES);
    lv_obj_set_style_radius(btn, BTN_VER_RES / 2, 0);
    lv_obj_center(btn);
}

static lv_obj_t* create_mode_select_item(lv_obj_t* parent, const char* text) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, MODE_SELECT_ITEM_HOR_RES, MODE_SELECT_ITEM_VER_RES);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* left_btn = lv_btn_create(panel);
    stylish_default_btn(left_btn, text);

    lv_obj_t* right_btn = lv_btn_create(panel);
    stylish_setting_btn(right_btn);

    return panel;
}

static void stylish_setting_btn(lv_obj_t* btn) {
    lv_obj_set_size(btn, SETTING_BTN_HOR_RES, SETTING_BTN_VER_RES);
    lv_obj_set_style_radius(btn, lv_pct(50), 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_SETTINGS, 0);
}


/* Utility function */

static void* init_message_package(void) {
    configuration_t* data = (configuration_t*) heap_caps_malloc(sizeof(configuration_t), MALLOC_CAP_DEFAULT);
    memcpy(data, &configuration, sizeof(configuration_t));
    return data;
}

static void power_btn_set_state(lv_obj_t* btn, bool state) {
    lv_obj_t* label = lv_obj_get_child(btn, 0);
    if (state) {
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREEN), 0);
        lv_label_set_text(label, "ON");
    }
    else {
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);
        lv_label_set_text(label, "OFF");
    }
}


/* Main function */

void init_gui(void)
{
    EVENT_SELECTED_CHANGED = lv_event_register_id();
    EVENT_CONFIRMED = lv_event_register_id();
    EVENT_APPLIED = lv_event_register_id();

    container = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));

    manager = lv_fragment_manager_create(NULL);
    lv_obj_add_event_cb(container, container_del_cb, LV_EVENT_DELETE, NULL);

    lv_obj_add_event_cb(lv_scr_act(), gesture_event_cb, LV_EVENT_GESTURE, NULL);

    lv_msg_subscribe(REFRESH_GUI, message_received_cb, NULL);
    lv_msg_subscribe(MSG_MATCH_RESULT, message_received_cb, NULL);
    lv_msg_subscribe(MSG_WIFI_SCAN_DONE, message_received_cb, NULL);
    lv_msg_subscribe(MSG_WIFI_CONNECTED, message_received_cb, NULL);
    lv_msg_subscribe(MSG_READ_RESULT, message_received_cb, NULL);
    lv_msg_subscribe(MSG_WRITE_RESULT, message_received_cb, NULL);

    show_loading_gui("System initializing...");
}

void refresh_gui(void) {
    lv_fragment_t* fragment = lv_fragment_manager_get_top(manager);
    lv_fragment_recreate_obj(fragment);
}

void clear_gui(void) {
    uint8_t size = lv_fragment_manager_get_stack_size(manager);
    for (uint8_t i = 0; i < size; i++) {
        lv_fragment_manager_pop(manager);
    }
}

void show_connect_gui(void) {
    lv_fragment_t* wifi_scan_fragment = lv_fragment_create(&wifi_scan_cls, NULL);
    lv_fragment_manager_push(manager, wifi_scan_fragment, &container);
}

void show_main_gui(void) {
    lv_fragment_t* main_gui_fragment = lv_fragment_create(&home_cls, NULL);
    lv_fragment_manager_push(manager, main_gui_fragment, &container);
}

void show_matching_failed_gui(void) {

}
