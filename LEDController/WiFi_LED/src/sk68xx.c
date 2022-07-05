/**
 * @file sk68xx.c
 * @author Jackie
 * @brief 三色LED灯驱动
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "sk68xx.h"

const static char* TAG = "sk68xx";

static uint32_t sk68xx_t0h_ticks = 0;
static uint32_t sk68xx_t1h_ticks = 0;
static uint32_t sk68xx_t0l_ticks = 0;
static uint32_t sk68xx_t1l_ticks = 0;

static void IRAM_ATTR sk68xx_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num);

/**
 * @brief 初始化LED灯
 * @param None
 * @return esp_err_t 调用状态
 */
esp_err_t sk68xx_init(void)
{
    esp_err_t ret;

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(SK68XX_GPIO_PIN, SK68XX_RMT_CHANNEL);
    config.clk_div = 2;  // 80MHz / 2 = 40MHz

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    uint32_t counter_clk_hz = 0;
    ret = rmt_get_counter_clock(SK68XX_RMT_CHANNEL, &counter_clk_hz);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "get rmt counter clock failed");
        return ESP_FAIL;
    }

    // ns -> ticks
    float ratio = (float)counter_clk_hz / 1e9;
    sk68xx_t0h_ticks = (uint32_t)(ratio * SK68XX_T0H_NS);
    sk68xx_t0l_ticks = (uint32_t)(ratio * SK68XX_T0L_NS);
    sk68xx_t1h_ticks = (uint32_t)(ratio * SK68XX_T1H_NS);
    sk68xx_t1l_ticks = (uint32_t)(ratio * SK68XX_T1L_NS);

    // set ws2812 to rmt adapter
    rmt_translator_init(SK68XX_RMT_CHANNEL, sk68xx_rmt_adapter);

    return ESP_OK;
}

/**
 * @brief RMT适配器(将RGB信息转换为RMT格式)
 * 
 * @param src 源数据
 * @param dest 目标数据
 * @param src_size 源数据数目
 * @param wanted_num 要获取的RMT数据数目
 * @param translated_size 已经被转换的数据数目
 * @param item_num 实际被转换的RMT数据数目
 */
static void IRAM_ATTR sk68xx_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num)
{
    if (src == NULL || dest == NULL) {
        *translated_size = 0;
        *item_num = 0;
        return;
    }
    const rmt_item32_t bit0 = {{{sk68xx_t0h_ticks, 1, sk68xx_t0l_ticks, 0}}}; //Logical 0
    const rmt_item32_t bit1 = {{{sk68xx_t1h_ticks, 1, sk68xx_t1l_ticks, 0}}}; //Logical 1
    size_t size = 0;
    size_t num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;
    while (size < src_size && num < wanted_num) {
        for (int i = 0; i < 8; i++) {
            // MSB first
            if (*psrc & (1 << (7 - i))) {
                pdest->val =  bit1.val;
            }
            else {
                pdest->val =  bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }
    *translated_size = size;
    *item_num = num;
}

/**
 * @brief 设置LED灯颜色
 * 
 * @param red 红色值
 * @param green 绿色值
 * @param blue 蓝色值
 * @param timeout_ms 最大等待时间
 * @return esp_err_t 调用状态
 */
esp_err_t sk68xx_set_color(uint8_t red, uint8_t green, uint8_t blue, uint32_t timeout_ms)
{
    uint8_t buf[3] = {green, red, blue};
    rmt_write_sample(SK68XX_RMT_CHANNEL, buf, 3, true);
    return rmt_wait_tx_done(SK68XX_RMT_CHANNEL, pdMS_TO_TICKS(timeout_ms));
}

/**
 * @brief 关闭所有LED灯
 * 
 * @param timeout_ms 最大等待时间
 * @return esp_err_t 调用状态
 */
esp_err_t sk68xx_clear(uint32_t timeout_ms)
{
    // Write zero to turn off all leds
    return sk68xx_set_color(0, 0, 0, timeout_ms);
}
