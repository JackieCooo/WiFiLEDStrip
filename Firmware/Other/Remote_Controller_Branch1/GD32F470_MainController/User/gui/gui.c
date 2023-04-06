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

static void stylish_default_btn(lv_obj_t* btn, const char* text);
static lv_obj_t* create_mode_select_item(lv_obj_t* parent, const char* text);
static void stylish_setting_btn(lv_obj_t* btn);


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


/* Callback functions */

static lv_obj_t* home_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    LV_UNUSED(self);

    lv_obj_t* content = lv_list_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 25, 0);
    lv_obj_set_style_pad_top(content, 20, 0);
    lv_obj_set_style_pad_bottom(content, 20, 0);
    lv_obj_set_style_border_opa(content, 0, 0);
    lv_obj_set_style_bg_opa(content, 0, 0);

    lv_obj_t* pwr_btn = lv_btn_create(content);
	if (configuration.power)
	{
		stylish_default_btn(pwr_btn, "ON");
		lv_obj_set_style_bg_color(pwr_btn, lv_palette_main(LV_PALETTE_GREEN), 0);
	}
	else
	{
		stylish_default_btn(pwr_btn, "OFF");
		lv_obj_set_style_bg_color(pwr_btn, lv_palette_main(LV_PALETTE_RED), 0);
	}
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
    LV_UNUSED(self);

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_remove_style_all(content);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));

    lv_obj_t* color_selector = lv_colorwheel_create(content, true);
    lv_colorwheel_set_mode(color_selector, LV_COLORWHEEL_MODE_HUE);
    lv_colorwheel_set_mode_fixed(color_selector, true);
	if (configuration.mode == MODE_NORMAL)
		lv_colorwheel_set_rgb(color_selector, configuration.setting.normal.color);
	else if (configuration.mode == MODE_BREATHING)
		lv_colorwheel_set_rgb(color_selector, configuration.setting.breathing.color);
	else if (configuration.mode == MODE_LIGHTBEAM)
		lv_colorwheel_set_rgb(color_selector, configuration.setting.lightbeam.color);
    lv_obj_set_size(color_selector, 320 * 0.75, 320 * 0.75);
    lv_obj_center(color_selector);
    lv_obj_add_event_cb(color_selector, color_selector_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    return content;
}

static lv_obj_t* mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    LV_UNUSED(self);

    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(content, lv_pct(100), lv_pct(100));
    lv_obj_set_style_pad_row(content, 25, 0);
    lv_obj_set_style_pad_top(content, 20, 0);
    lv_obj_set_style_pad_bottom(content, 20, 0);
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
	LV_UNUSED(self);
	
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
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 20, 0);
    lv_obj_set_style_pad_bottom(content, 20, 0);
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
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_APPLIED, fragment);

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
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 20, 0);
    lv_obj_set_style_pad_bottom(content, 20, 0);
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
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_APPLIED, fragment);

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
    lv_obj_set_style_pad_row(content, 15, 0);
    lv_obj_set_style_pad_top(content, 20, 0);
    lv_obj_set_style_pad_bottom(content, 20, 0);
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
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_CONFIRMED, fragment);
    lv_obj_add_event_cb(confirm_panel, confirm_panel_event_cb, EVENT_CONFIRM_PANEL_APPLIED, fragment);

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
        lv_obj_t* btn = (lv_obj_t*) lv_event_get_target(e);
        configuration.power = !configuration.power;
        if (configuration.power)
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
		
		MSG_SendCmd();
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
        printf("color: #%04X\n", lv_color_to16(color));
		
		if (configuration.mode == MODE_NORMAL)
			configuration.setting.normal.color = color;
		else if (configuration.mode == MODE_BREATHING)
			configuration.setting.breathing.color = color;
		else if (configuration.mode == MODE_LIGHTBEAM)
			configuration.setting.lightbeam.color = color;
		MSG_SendCmd();
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
        printf("mode: %d\n", *cur_sel);

		configuration.mode = *cur_sel;
		MSG_SendCmd();
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

static void confirm_panel_event_cb(lv_event_t* e) {
    if (e->code == EVENT_CONFIRM_PANEL_CONFIRMED || e->code == EVENT_CONFIRM_PANEL_APPLIED) {
        lv_obj_t* obj = lv_event_get_target(e);
        led_mode_t* mode = (led_mode_t*) lv_obj_get_user_data(obj);
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

        MSG_SendCmd();
    }
    if (e->code == EVENT_CONFIRM_PANEL_CONFIRMED) {
        lv_fragment_manager_pop(manager);
    }
}


/* Style functions */

static void stylish_default_btn(lv_obj_t* btn, const char* text)
{
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_set_size(btn, DEFAULT_BTN_W, DEFAULT_BTN_H);
    lv_obj_set_style_radius(btn, DEFAULT_BTN_H / 2, 0);
    lv_obj_center(btn);
}

static lv_obj_t* create_mode_select_item(lv_obj_t* parent, const char* text) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, MODE_SELECT_ITEM_W, MODE_SELECT_ITEM_H);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* left_btn = lv_btn_create(panel);
    stylish_default_btn(left_btn, text);

    lv_obj_t* right_btn = lv_btn_create(panel);
    stylish_setting_btn(right_btn);

    return panel;
}

static void stylish_setting_btn(lv_obj_t* btn) {
    lv_obj_set_size(btn, SETTING_BTN_W, SETTING_BTN_H);
    lv_obj_set_style_radius(btn, lv_pct(50), 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_SETTINGS, 0);
}


/* Main function */

void create_gui(void)
{
	init_configuration();
	
    EVENT_SELECTED_CHANGED = lv_event_register_id();
	confirm_panel_register_event_id();

    container = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));

    manager = lv_fragment_manager_create(NULL);
    lv_obj_add_event_cb(container, container_del_cb, LV_EVENT_DELETE, NULL);

    lv_fragment_t* main_gui_fragment = lv_fragment_create(&home_cls, NULL);
    lv_fragment_manager_push(manager, main_gui_fragment, &container);

    lv_obj_add_event_cb(lv_scr_act(), gesture_event_cb, LV_EVENT_GESTURE, NULL);
}
