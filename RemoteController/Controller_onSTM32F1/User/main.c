/**
  ******************************************************************************
  * @file    main.c 
  * @author  Jackie
  * @version V1.0
  * @date    2022-07-05
  * @brief   WiFiµÆ´ø¿ØÖÆÆ÷
  ******************************************************************************
  */

#include "stm32f10x.h"
#include <stdio.h>
#include "./usart/usart.h"
#include "./systick/systick.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "./gui/led_controller.h"
#include "./esp8266/esp8266.h"
#include "./esp8266/esp8266_app.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SysTick_Init();
	ILI9341_Init();
	XPT2046_Init();
	USART_Config();
	ESP8266_Init();
	ESP8266_AppInit();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	interface_init();

	while(1)
	{
		lv_task_handler();
		Delay_ms(5);
	}
}
