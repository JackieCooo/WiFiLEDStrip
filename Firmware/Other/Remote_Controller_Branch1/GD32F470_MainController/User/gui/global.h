#ifndef LVGL_GLOBAL_H
#define LVGL_GLOBAL_H

#define SCREEN_HOR_RES 240
#define SCREEN_VER_RES 240

typedef enum {
    MODE_NORMAL,
    MODE_BREATHING,
    MODE_LIGHTBEAM,
    MODE_RAINBOW
} led_mode_t;

typedef struct {
    uint8_t faded_head : 1;
    uint8_t faded_tail : 1;
} faded_end_t;

#endif //LVGL_GLOBAL_H
