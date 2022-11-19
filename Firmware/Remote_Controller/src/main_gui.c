#include "main_gui.h"

/* User defines */
#define COLOR_CHANGE_MSG 1

/* Enums define */

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_INTERVAL,
    MODE_RAINBOW
} led_mode_t;


/* Static variables define */

static lv_obj_t* container;
static lv_timer_t* select_timer;
static lv_fragment_manager_t* manager;
static lv_fragment_t* main_gui_fragment;
static lv_fragment_t* color_select_fragment;
static lv_fragment_t* mode_select_fragment;
static lv_fragment_t* about_page_fragment;

static uint8_t pwr = 0;
static lv_color_t cur_color;
static led_mode_t cur_mode = MODE_NORMAL;


/* Static functions define */

static lv_obj_t * main_gui_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t * color_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static lv_obj_t * mode_select_create_cb(lv_fragment_t* self, lv_obj_t* parent);
static void container_del_cb(lv_event_t* e);
static void pwr_btn_event_cb(lv_event_t* e);
static void color_btn_event_cb(lv_event_t* e);
static void mode_btn_event_cb(lv_event_t* e);
static void about_btn_event_cb(lv_event_t* e);
static void selector_timer_cb(lv_timer_t* t);
static void color_changed_cb(lv_event_t* e);

static void stylize_regular_btn(lv_obj_t* btn, const char* text);
static void stylize_pwr_btn(lv_obj_t* btn);
static void stylize_color_btn(lv_obj_t* btn);


/* Static structs define */


typedef struct {
    lv_fragment_t base;
    lv_obj_t* pwr_btn;
    lv_obj_t* color_btn;
    lv_obj_t* mode_btn;
    lv_obj_t* about_btn;
} main_gui_fragment_t;

static const lv_fragment_class_t main_gui_cls = {
    .create_obj_cb = main_gui_create_cb,
    .instance_size = sizeof(main_gui_fragment_t)
};

typedef struct {
    lv_fragment_t base;
    lv_obj_t* color_selector;
} color_select_fragment_t;

static const lv_fragment_class_t color_select_cls = {
    .create_obj_cb = color_select_create_cb,
    .instance_size = sizeof(color_select_fragment_t)
};

typedef struct {
    lv_fragment_t base;
    lv_obj_t* normal_mode;
    lv_obj_t* breathing_mode;
    lv_obj_t* interval_mode;
    lv_obj_t* rainbow_mode;
} mode_select_fragment_t;

static const lv_fragment_class_t mode_select_cls = {
    .create_obj_cb = mode_select_create_cb,
    .instance_size = sizeof(mode_select_fragment_t)
};


/* Callback functions */

static lv_obj_t * main_gui_create_cb(lv_fragment_t* self, lv_obj_t* parent)
{
    main_gui_fragment_t* fragment = (main_gui_fragment_t*) self;

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
    stylize_regular_btn(fragment->pwr_btn, "OFF");
    stylize_pwr_btn(fragment->pwr_btn);
    lv_obj_add_event_cb(fragment->pwr_btn, pwr_btn_event_cb, LV_EVENT_CLICKED, fragment);

    fragment->color_btn = lv_btn_create(list);
    stylize_regular_btn(fragment->color_btn, "COLOR");
    stylize_color_btn(fragment->color_btn);
    lv_obj_add_event_cb(fragment->color_btn, color_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(fragment->color_btn, color_btn_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(COLOR_CHANGE_MSG, fragment->color_btn, NULL);

    fragment->mode_btn = lv_btn_create(list);
    stylize_regular_btn(fragment->mode_btn, "MODE");
    lv_obj_add_event_cb(fragment->mode_btn, mode_btn_event_cb, LV_EVENT_CLICKED, NULL);

    fragment->about_btn = lv_btn_create(list);
    stylize_regular_btn(fragment->about_btn, "ABOUT");
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
    stylize_regular_btn(fragment->normal_mode, "Normal");

    fragment->breathing_mode = lv_btn_create(list);
    stylize_regular_btn(fragment->breathing_mode, "Breathing");

    fragment->interval_mode = lv_btn_create(list);
    stylize_regular_btn(fragment->interval_mode, "Interval");

    fragment->rainbow_mode = lv_btn_create(list);
    stylize_regular_btn(fragment->rainbow_mode, "Rainbow");

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
        select_timer = lv_timer_create(selector_timer_cb, 3000, NULL);
    }
    else if (e->code == LV_EVENT_MSG_RECEIVED)
    {
        lv_obj_t * btn = lv_event_get_target(e);
        lv_msg_t * m = lv_event_get_msg(e);
        const uint16_t * c = lv_msg_get_payload(m);
        lv_obj_set_style_bg_color(btn, lv_color_hex(*c), 0);
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

}

static void selector_timer_cb(lv_timer_t* t)
{
    lv_msg_send(COLOR_CHANGE_MSG, &cur_color);
    lv_fragment_manager_pop(manager);
    lv_timer_del(select_timer);
}

static void color_changed_cb(lv_event_t* e)
{
    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t* color_wheel = (lv_obj_t*) lv_event_get_target(e);

        lv_timer_pause(select_timer);
        cur_color = lv_colorwheel_get_rgb(color_wheel);
        printf("color: 0x%x\n", lv_color_to16(cur_color));
        lv_timer_reset(select_timer);
        lv_timer_resume(select_timer);
    }
}


/* Style functions */

static void stylize_regular_btn(lv_obj_t* btn, const char* text)
{
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    lv_obj_set_size(btn, 120, 50);
    lv_obj_center(btn);
}

static void stylize_pwr_btn(lv_obj_t* btn)
{
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);
}

static void stylize_color_btn(lv_obj_t* btn)
{
    lv_obj_set_style_bg_color(btn, cur_color, 0);
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
