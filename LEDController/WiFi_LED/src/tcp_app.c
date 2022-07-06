/**
 * @file tcp_app.c
 * @author Jackie
 * @brief TCP协议栈驱动
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "tcp_app.h"

const static char* TAG = "TCP";

xQueueHandle data_received = NULL;

static void tcp_event_handler(tcp_event_base_t base, tcp_event_id_t id, void* param)
{
    if (base == TCP_SERVER)
    {
        if (id == TCP_SERVER_RECEIVED_DATA)
        {
            // ESP_LOGI(TAG, "Sending LED message");
            xQueueSend(data_received, param, portMAX_DELAY);  // 发送至任务队列
            // ESP_LOGI(TAG, "LED message sent");
        }
    }
}

#if (TCP_MODE == 1 || TCP_MODE == 3)

static void tcp_server_task(void* param);

/**
 * @brief TCP服务端初始化
 * @param sock 绑定的socket号
 * @return esp_err_t 调用状态
 */
esp_err_t tcp_server_init(int* sock)
{
    int addr_family = IP_PROTOCOL == 1 ? AF_INET : AF_INET6;
    int ip_protocol = 0;
    struct sockaddr_storage dest_addr;

    if (addr_family == AF_INET)
    {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }
    #if (IP_PROTOCOL == 2)
    else if (addr_family == AF_INET6)
    {
        struct sockaddr_in6 *dest_addr_ip6 = (struct sockaddr_in6 *)&dest_addr;
        bzero(&dest_addr_ip6->sin6_addr.un, sizeof(dest_addr_ip6->sin6_addr.un));
        dest_addr_ip6->sin6_family = AF_INET6;
        dest_addr_ip6->sin6_port = htons(PORT);
        ip_protocol = IPPROTO_IPV6;
    }
    #endif

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);  // 创建套接字
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // 允许本地地址复用
    #if (IP_PROTOCOL == 3)
    // Note that by default IPV6 binds to both protocols, it is must be disabled
    // if both protocols used at the same time (used in CI)
    setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
    #endif

    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));  // 绑定套接字到指定地址
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG, "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    *sock = listen_sock;

    return ESP_OK;

    CLEAN_UP:
    close(listen_sock);
    return ESP_FAIL;
}

/**
 * @brief 开启TCP服务端
 * 
 * @param sock 绑定的socket号
 * @return esp_err_t 调用状态
 */
esp_err_t start_tcp_server(int* sock)
{
    int err = listen(*sock, 1);  // 监听端口
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    xTaskCreate(tcp_server_task, "tcp_server_task", 4096, sock, 5, NULL);

    return ESP_OK;

    CLEAN_UP:
    close(*sock);
    return ESP_FAIL;
}

/**
 * @brief TCP服务端任务
 * 
 * @param param 用户参数
 * @return None
 */
static void tcp_server_task(void* param)
{
    int* listen_sock = (int*)param;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    char addr_str[128];
    uint8_t rx_buffer[RX_BUF_SIZE];
    data_received = xQueueCreate(3, sizeof(rx_buffer));

    while(true)
    {
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(*listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));  // 保持TCP连接
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));  // Keep-alive空闲时间
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));  // Keep-alive间隔时间
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));  // Keep-alive包最大重发次数
        // Convert ip address to string
        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
        #if (IP_PROTOCOL == 2 || IP_PROTOCOL == 3)
        else if (source_addr.ss_family == PF_INET6) {
            inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
        }
        #endif
        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);  // 接收数据
        if (len < 0) {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
        }
        else if (len == 0) {
            ESP_LOGW(TAG, "Connection closed");
        }
        else {
            ESP_LOGI(TAG, "Received %d bytes", len);
            for (uint8_t i = 0; i < len; i++)
            {
                printf("%x ", rx_buffer[i]);
            }
            printf("\n");
            tcp_event_handler(TCP_SERVER, TCP_SERVER_RECEIVED_DATA, (void*)rx_buffer);  // 处理数据接收成功事件
        }

        shutdown(sock, 0);
        close(sock);
    }

    vTaskDelete(NULL);
}
#endif
