#include "indev.h"

FocalTech_Class touch = FocalTech_Class(FT6236_SDA, FT6236_SCL);
lv_indev_t * indev_touchpad;

static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        // Serial.println("Touched");
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/
    if (touch.getTouched()) return true;
    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/
    uint16_t t_x, t_y;
    if(touch.getPoint(t_x, t_y))
    {
        if (t_x >= MY_DISP_HOR_RES) {
            t_x = MY_DISP_HOR_RES;
        }
        if (t_y >= MY_DISP_VER_RES) {
            t_y = MY_DISP_VER_RES;
        }
        (*x) = MY_DISP_HOR_RES - t_x;
        (*y) = MY_DISP_VER_RES - t_y;
    }
    else
    {
        (*x) = 0;
        (*y) = 0;
    }
    // Serial.printf("x: %d, y: %d\n", *x, *y);
}

void Indev::init(void)
{
    static lv_indev_drv_t indev_drv;

    touch.begin();
    Serial.printf("VID: 0x%x\n", touch.getVendorID());

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}
