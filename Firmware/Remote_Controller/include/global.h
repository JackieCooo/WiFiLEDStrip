#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "freertos/queue.h"
#include "lwip/ip4_addr.h"
#include "list.h"

#define MSG_MATCH_RESULT                2
#define MSG_ACK_RESULT                  3
#define MSG_WIFI_SCAN_DONE              4
#define MSG_WIFI_CONNECTED              5
#define MSG_READ_RESULT                 6
#define MSG_WRITE_RESULT                7
#define MSG_WIFI_DISCONNECTED           8
#define MSG_WIFI_PASSWORD_FAILED        9

#define QUEUE_TIMEOUT_MS                (20)

typedef enum {
    MSG_READ_HOST,
    MSG_WRITE_HOST,
    MSG_MATCH_HOST,
    MSG_WIFI_SCAN,
    MSG_WIFI_CONNECT,
    MSG_ACK_HOST,
} msg_t;

typedef struct {
    msg_t msg;
    void* user_data;
} msg_request_t;

typedef struct {
    msg_t msg;
    void* user_data;
    bool resp;
} msg_reply_t;

#define SSID_MAX_LEN    32
#define PSW_MAX_LEN     32

typedef struct {
    ip4_addr_t host_ip;
    char ssid[SSID_MAX_LEN];
    char password[PSW_MAX_LEN];
} connectivity_t;

#define MAX_WIFI_LIST_LEN 32

typedef struct {
    char ssid[SSID_MAX_LEN];
    int16_t rssi;
} wifi_info_t;

typedef list_t wifi_list_t;

typedef struct {
    char ssid[SSID_MAX_LEN];
    char password[PSW_MAX_LEN];
} wifi_connect_t;

extern configuration_t configuration;
extern connectivity_t connectivity;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
