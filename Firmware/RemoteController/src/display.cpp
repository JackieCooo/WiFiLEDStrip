#include "display.h"

TFT_eSPI screen = TFT_eSPI();

static void display_init(void);
static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);

static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    screen.startWrite();
    screen.setAddrWindow(area->x1, area->y1, w, h);
    screen.pushColors((uint16_t*)color_p, w*h);
    screen.endWrite();

	lv_disp_flush_ready(disp);
}

static void display_init(void) {
    screen.init();
    screen.fillScreen(TFT_BLACK);
}

void Display::init(void)
{
    display_init();

    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 20];                        /*A buffer for 10 rows*/
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 20];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 20);   /*Initialize the display buffer*/

    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
    // lv_color_t* buf_3_1 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES, MALLOC_CAP_SPIRAM);
    // lv_color_t* buf_3_2 = (lv_color_t*) heap_caps_malloc(MY_DISP_HOR_RES * MY_DISP_VER_RES, MALLOC_CAP_SPIRAM);
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_VER_RES * MY_DISP_HOR_RES);   /*Initialize the display buffer*/
    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = my_disp_flush;
    // disp_drv.full_refresh = 1;
    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}
