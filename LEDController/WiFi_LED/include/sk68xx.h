#ifndef __SK68XX_H_
#define __SK68XX_H_

#include "driver/rmt.h"
#include "esp_log.h"

#define SK68XX_T0H_NS (300)
#define SK68XX_T0L_NS (900)
#define SK68XX_T1H_NS (600)
#define SK68XX_T1L_NS (600)
#define SK68XX_RESET_US (80)

#define SK68XX_RMT_CHANNEL RMT_CHANNEL_0
#define SK68XX_GPIO_PIN 18

typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} sk68xx_color_t;

esp_err_t sk68xx_init(void);
esp_err_t sk68xx_set_color(uint8_t red, uint8_t green, uint8_t blue, uint32_t timeout_ms);
esp_err_t sk68xx_clear(uint32_t timeout_ms);

#endif
