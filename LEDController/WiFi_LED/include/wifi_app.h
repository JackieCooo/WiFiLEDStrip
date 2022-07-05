#ifndef _WIFI_APP_H
#define _WIFI_APP_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_MODE 2  /* 1. 基站模式(STA) 2. 热点模式(SoftAP) 3. 双模模式 */

#if (WIFI_MODE == 1 || WIFI_MODE == 3)

#define WIFI_SSID      "HUAWEI-10GJ0E"
#define WIFI_PASS      "abc12345678"
#define MAXIMUM_RETRY  5

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#elif (WIFI_MODE == 2 || WIFI_MODE == 3)

#define ESP_WIFI_SSID      "ESP_WIFI"
#define ESP_WIFI_PASS      "12345678"
#define WIFI_CHANNEL       1
#define MAX_STA_CONN       1

#endif


void nvs_init(void);
void wifi_app_init(void);
#if (WIFI_MODE == 1 || WIFI_MODE == 3)
void wifi_init_sta(void);
#elif (WIFI_MODE == 2 || WIFI_MODE == 3)
void wifi_init_softap(void);
#endif

#endif