/**
  ******************************************************************************
  * @file    main.c 
  * @author  Jackie
  * @version V1.0
  * @date    2023-02-24
  * @brief   Smartlight Remote Controller Firmware
  ******************************************************************************
  */

#include "gd32f4xx.h"
#include "debug.h"
#include "esp8266/esp8266.h"
#include "st7796/st7796.h"
#include "gt911/gt911.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

static uint8_t state = 0;

static void btn_event_cb(lv_event_t* e) {
  if (e->code == LV_EVENT_CLICKED) {
    lv_obj_t* btn = lv_event_get_target(e);
    if (state) {
      lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
    }
    else {
      lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);
    }
    state = ~state;
  }
}

static void create_gui(void) {
  lv_obj_t* btn = lv_btn_create(lv_scr_act());
  lv_obj_center(btn);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text(label, "Test");
  lv_obj_center(label);
}

static void LvglTask(void* args)
{
	for(;;)
	{
		lv_task_handler();
		lv_tick_inc(5);
		vTaskDelay(5 / portTICK_PERIOD_MS);
	}
}

static void AppInitTask(void* args)
{
	taskENTER_CRITICAL();

	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

	DEBUG_Init();
	ESP8266_Init();
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	vTaskDelay(50);
	create_gui();
	
	xTaskCreate(LvglTask, "LvglTask", 1024, NULL, 8, NULL);
	xTaskCreate(ESP8266_Task, "ESP8266_Task", 512, NULL, 5, NULL);
	
	taskEXIT_CRITICAL();
	
	vTaskDelete(NULL);
}

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	xTaskCreate(AppInitTask, "AppInitTask", 1024, NULL, 5, NULL);
	vTaskStartScheduler();

    while(1);
}
