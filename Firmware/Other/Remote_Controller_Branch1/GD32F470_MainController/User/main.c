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
#include "gui.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

static void LvglTask(void* args)
{
	create_gui();
	
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
	CommandDataQueue = xQueueCreate(3, sizeof(esp8266_buf_t));

	DEBUG_Init();
	ESP8266_Init();
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	vTaskDelay(50);
	
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
