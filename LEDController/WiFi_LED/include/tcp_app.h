#ifndef _TCP_APP_H_
#define _TCP_APP_H_

#include <stdio.h>
#include <string.h>
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define IP_PROTOCOL 1  /* 1. IPV4 2. IPV6 3. IPV4&IPV6*/
#define TCP_MODE 1  /* 1. 服务端模式 2. 客户端模式 3. 双模模式 */
#define RX_BUF_SIZE 16  /* 接收缓冲区大小 */

#if (TCP_MODE == 1 || TCP_MODE == 3)

#define PORT                  12345
#define KEEPALIVE_IDLE        5
#define KEEPALIVE_INTERVAL    5
#define KEEPALIVE_COUNT       3

typedef enum {
    TCP_SERVER,
    TCP_CLIENT,
    TCP_DAUL,
} tcp_event_base_t;

typedef enum {
    TCP_SERVER_RECEIVED_DATA,
    TCP_SERVER_SENT_DATA,
    TCP_CLIENT_RECEIVED_DATA,
    TCP_CLIENT_SENT_DATA,
} tcp_event_id_t;

extern xQueueHandle data_received;

#elif (TCP_MODE == 2 || TCP_MODE == 3)

#endif


#if (TCP_MODE == 1 || TCP_MODE == 3)
esp_err_t tcp_server_init(int* sock);
esp_err_t start_tcp_server(int* sock);
#elif (TCP_MODE == 2 || TCP_MODE == 3)
#endif

#endif
