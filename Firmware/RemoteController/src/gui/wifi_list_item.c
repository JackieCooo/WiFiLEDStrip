#include "gui/wifi_list_item.h"

lv_obj_t* create_wifi_list_item(lv_obj_t* parent, char* ssid) {
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, lv_pct(100), WIFI_LIST_ITEM_H);
    lv_obj_set_style_border_side(panel, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_opa(panel, lv_pct(50), 0);
    lv_obj_set_style_border_color(panel, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(panel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(panel, ssid);

    lv_obj_t* ssid_label = lv_label_create(panel);
    lv_label_set_text(ssid_label, ssid);

    return panel;
}
