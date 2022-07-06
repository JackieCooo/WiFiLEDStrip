#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_task_wdt.h"

#include <stdio.h>
#include "esp_log.h"

#include "sk68xx.h"
#include "wifi_app.h"
#include "tcp_app.h"

const static char* TAG = "APP";

static int sock = -1;

/**
 * @brief LED控制任务
 * @param params 用户参数
 * @return None
 */
static void led_task(void* params)
{
    uint8_t buf[RX_BUF_SIZE];

    while(true)
    {
        if (data_received != NULL && xQueueReceive(data_received, &buf, portMAX_DELAY))
        {
            // ESP_LOGI(TAG, "Got LED message");
            
            uint8_t status = buf[0];
            uint8_t red = buf[1];
            uint8_t green = buf[2];
            uint8_t blue = buf[3];

            if (status > 1) continue;

            // ESP_LOGI(TAG, "Message: %x, %x, %x, %x", status, red, green, blue);

            if (status) ESP_ERROR_CHECK(sk68xx_set_color(red, green, blue, 100));
            else ESP_ERROR_CHECK(sk68xx_clear(100));
        }
    }

    vTaskDelete(NULL);
}

/**
 * @brief 外设初始化任务
 * @param params 用户参数
 * @return None
 */
static void app_init_task(void* params)
{
    wifi_app_init();  // 初始化并开启WiFi
    ESP_ERROR_CHECK(tcp_server_init(&sock));  // 初始化TCP服务端
    ESP_ERROR_CHECK(start_tcp_server(&sock));  // 开启TCP服务端
    ESP_ERROR_CHECK(sk68xx_init());  // 初始化LED灯带
    ESP_ERROR_CHECK(sk68xx_clear(100));

    xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);

    vTaskDelete(NULL);
}

/**
 * @brief 主函数
 * @param None
 * @return None
 */
void app_main(void)
{
    ESP_LOGI(TAG, "WiFi灯带测试");
    xTaskCreate(app_init_task, "app_init_task", 4096, NULL, 5, NULL);
}
