#include "display.h"

#define MY_DISP_HOR_RES 240
#define MY_DISP_VER_RES 240
#define LV_VER_RES_MAX MY_DISP_VER_RES

Arduino_ST7789 screen = Arduino_ST7789(ST7789_DC, ST7789_RST, ST7789_MOSI, ST7789_SCLK, ST7789_CS, ST7789_BLC);

static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);

static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;
    screen.pushColors(area->x1, area->y1, w, h, (uint16_t*)color_p);

	lv_disp_flush_ready(disp);
}

void Display::init(void)
{
    screen.init();

    static lv_disp_draw_buf_t draw_buf_dsc_3;
    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = my_disp_flush;
    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_3;
    /*Required for Example 3)*/
    disp_drv.full_refresh = 1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}
